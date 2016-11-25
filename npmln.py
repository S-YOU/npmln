#!/usr/bin/env python
# coding: utf8

__version__ = '0.6.0'

__line_size = 0

def main():
	import argparse
	parser = argparse.ArgumentParser()
	parser.add_argument("commands", nargs="+", help="npmln commands (install)")

	parser.add_argument("-g", action="store_true", help="symlink bin globally into (--prefix), with sudo or use --no-sudo")
	parser.add_argument("--no-sudo", action="store_true", default=False, help="don't use sudo, when you are root inside docker (%(default)s)")

	parser.add_argument("--base", default=".", help="base directory for install (%(default)s), lookup package.json")
	parser.add_argument("--prefix", default="/usr/local", help="global bin scripts will be installed into (%(default)s)/bin/")

	parser.add_argument("--pkg-dir", default="/var/tmp/npmln", help="Package directory to be installed (%(default)s)")
	parser.add_argument("--tmp-dir", default="/dev/shm", help="tmp folder to download gzipped tarballs (%(default)s)")

	parser.add_argument("--no-prod", action="store_true", default=False, help="don't install dependencies and peerDependencies (%(default)s)")
	parser.add_argument("--dev", action="store_true", default=False, help="install devDependencies (%(default)s)")

	parser.add_argument("--semver-patch", action="store_true", default=False, help="respect semver patch (%(default)s)")
	parser.add_argument("--semver-minor", action="store_true", default=False, help="respect semver minor (%(default)s)")
	parser.add_argument("--retries", type=int, default=20, help="retries when url read failed (%(default)s)")
	parser.add_argument("-j", type=int, default=64, help="number of threads (%(default)s)")
	parser.add_argument("--reinstall", action="store_true", default=False, help="reinstall specified module(s) regardless of link exists (%(default)s)")
	parser.add_argument("--allow-node-modules", action="store_true", default=False, help="allow node_modules folder to be included in package (%(default)s)")

	parser.add_argument("--no-cache", action="store_true", default=False, help="no cache for downloaded tarballs (%(default)s)")
	parser.add_argument("--cache-dir", default="/var/tmp/npmln-cache", help="cache folder for tarballs (%(default)s)")
	parser.add_argument("--dump", action="store_true", default=False, help="create list of install files (%(default)s)")

	parser.add_argument("-v", action="version", version="npmln (version %s)" % __version__)

	# parser.print_help()

	args = parser.parse_args()
	commands = args.commands
	cmd0, cmdn = commands[0], commands[1:]
	sudo = '' if args.no_sudo else 'sudo '
	# print args

	import sys
	import os
	import cjson

	listdir, isdir, islink, join, exists, realpath, relpath = os.listdir, os.path.isdir, os.path.islink, os.path.join, os.path.exists, os.path.realpath, os.path.relpath
	downloaded = set()

	def errwrite(*arg):
		global __line_size
		line = ' '.join(str(x) for x in arg)
		_len = len(line)
		if len(line) < __line_size:
			line += " " * (__line_size - len(line))
		__line_size = _len
		sys.stderr.write("\r" + line + "\n")
		sys.stderr.flush()

	if "install".startswith(cmd0):
		import xxhash
		import subprocess
		import rc_semver
		import urllib
		import time

		if not exists(args.pkg_dir):
			os.mkdir(args.pkg_dir)
		if not exists(args.cache_dir):
			os.mkdir(args.cache_dir)

		TREE = "|   "
		def tree(lvl, newLine=False, *args):
			global __line_size
			line = ""
			if not newLine:
				line += "\r"
				lvl = 0
			sys.stdout.write(" ")
			if lvl > 0:
				line += TREE * (lvl - 1)
				line += "`---"
			line += ' '.join(str(x) for x in args)
			if not newLine:
				_len = len(line)
				if len(line) < __line_size:
					line += " " * (__line_size - len(line))
				__line_size = _len
			else:
				line += "\n"
			sys.stdout.write(line)
			sys.stdout.flush()

		# directly download package
		def load_url(url, pkg_path, mod, ver):
			if url.endswith((".tar.gz", ".tgz")):
				tmp_file = "%s/%s-%s.tgz" % (args.tmp_dir if args.no_cache else args.cache_dir, mod, ver)
				if tmp_file in downloaded:
					return
				excludes = ["test/", "doc/"]
				if not args.allow_node_modules:
					excludes.append("node_modules/")
				if not exists(tmp_file):
					print "downloading", [mod, ver, pkg_path, tmp_file]
					cmd = 'curl --compressed --connect-timeout 1 --retry 10 --retry-delay 0 -A "npm/4.0.3 node/v7.1.0 linux x64" -sL %s > %s' % (url, tmp_file)
					cmd += " && mkdir -p %s && tar zxf %s -C %s --strip-components 1 --warning=no-unknown-keyword %s" % (
						pkg_path, tmp_file, pkg_path, ''.join(" --exclude=%s" % x for x in excludes))
					if args.no_cache:
						cmd += " && rm -f %s" % tmp_file
				else:
					#print "cache:", [mod, ver, pkg_path, tmp_file]
					cmd = "mkdir -p %s && tar zxf %s -C %s --strip-components 1 --warning=no-unknown-keyword %s" % (
						pkg_path, tmp_file, pkg_path, ''.join(" --exclude=%s" % x for x in excludes))
				downloaded.add(tmp_file)

				for i in range(1, args.retries + 1):
					ret = subprocess.call(cmd, shell=True)
					if not ret: break
					if i == args.retries:
						errwrite("[ERR] url: %s, retval: %d, retried %d, TIMEOUT" % (url, ret, i))
					else:
						errwrite("[%s] url: %s, retval: %d, retrying %d/10 .... " % (
							['WARN','INFO'][i < args.retries / 2], url, ret, i))
						time.sleep(1 if "github" in url else 0)

		# search in repository
		def npm_pull(mod, ver_pattern, mod_base, lvl, prev_path):
			counter[0] += 1
			pair = mod, ver_pattern
			new_pkg_path = None
			#print "\n", pair, "\n"

			if mod[0] == "@":
				mod = mod.replace('/', '%2f')

			if ver_pattern.startswith("http"):
				#http.*xxx.git#yyy -> .tar.gz
				if ".git#" in ver_pattern:
					ver_pattern = "%s/archive/%s.tar.gz" % tuple(ver_pattern.split(".git#",1))
					#print[ver_pattern]
				#http.*xxx.git -> .tar.gz
				elif ver_pattern.endswith(".git"):
					ver_pattern = ver_pattern[:-4] + "%s/archive/master.tar.gz"

			#username/repo
			elif '/' in ver_pattern:
				ver_pattern = 'http://github.com/' + ver_pattern + '/archive/master.tar.gz'

			#http.*xxx.tar.gz
			if ver_pattern.endswith("tar.gz"):
				real_ver = xxhash.xxh64(ver_pattern).hexdigest()
				new_pkg_path = join(mod_base, real_ver)
				tarball_path = ver_pattern
				pkg_json = None
				if uniq_modules.get(pair) is None:
					uniq_modules[pair] = new_pkg_path
				#print ["tar.gz", new_pkg_path]
			else:
				# print mod, ver_pattern, mod_base, lvl
				u = urllib.urlopen("https://registry.yarnpkg.com/%s/%s" % (mod, ver_pattern or '*'))
				pkg_content = u.read()
				try:
					pkg_json = cjson.decode(pkg_content)
				except:
					return errwrite("\nERR:npm pull: %r, invalid package.json?" % [mod, ver_pattern, pkg_content])
				if pkg_json and "dist" in pkg_json and "tarball" in pkg_json["dist"]:
					real_ver = pkg_json["version"]
					new_pkg_path = join(mod_base, real_ver)
					tarball_path = pkg_json["dist"]["tarball"].replace("http://registry.npmjs.org/", "https://registry.yarnpkg.com/")
					if uniq_modules.get(pair) is None:
						uniq_modules[pair] = new_pkg_path
				else:
					# print "no package path", [pkg_json, mod, ver_pattern]
					return errwrite("\nERR:npm pull: %r, invalid module?" % [mod, ver_pattern, pkg_json])
				# errwrite(new_pkg_path, pkg_json["dist"]["tarball"])
			#print new_pkg_path

			#print "npmpull", [mod, ver_pattern, mod_base, lvl, prev_path]
			is_new = not exists(new_pkg_path) or (cmdn and args.reinstall)
			real_pair = mod, real_ver
			if is_new:
				load_url(tarball_path, new_pkg_path, mod, real_ver)

			if pkg_json is None:
				pkg_json = cjson.decode(open(join(new_pkg_path, "package.json")).read())

			if is_new:
				pkg_scripts = pkg_json.get("scripts")
				if pkg_scripts:
					if "preinstall" in pkg_scripts:
						# print "preinstall:", pkg_scripts["preinstall"],
						subprocess.call("cd %s && npm run preinstall" % new_pkg_path, shell=True)
					if pkg_scripts and ("preinstall" in pkg_scripts or "install" in pkg_scripts or "postinstall" in pkg_scripts):
						rebuilds[real_pair] = new_pkg_path, []
						if "install" in pkg_scripts:
							rebuilds[real_pair][1].append("install")
						if "postinstall" in pkg_scripts:
							rebuilds[real_pair][1].append("postinstall")

			# add to relink submodules, or when running directly
			if not pair in relinks:
				relinks[pair] = []
				sub_pkg_path = os.path.join(prev_path, 'node_modules', mod)
				relinks[pair].append((sub_pkg_path, lvl))

			# print "is_new", is_new, mod, ver_pattern, new_pkg_path, uniq_modules

			if is_new:
				if pair not in uniq_modules:
					uniq_modules[pair] = new_pkg_path
				install_recur(new_pkg_path, lvl, prev_path)

			counter[1] += 1

		def semver_diff(pattern, ver):
			t, p3, ver3 = pattern[0], pattern[1:4], ver[1:4]
			if t == '>':
				if ver3 > p3: return True
			elif t == '>=':
				if ver3 >= p3: return True
			elif t == '^':
				# ^1.2.3 := >=1.2.3 <2.0.0
				if ver3 >= p3 and ver3 < (p3[0] + 1, 0, 0): return True
			elif t == '~':
				# ~1.2.3 := >=1.2.3 <1.3.0
				if ver3 >= p3 and ver3 < (p3[0], p3[1] + 1 if p3[1] else '@', 0): return True
			elif t == '' or t == '=':
				if \
				(p3[0] is None or ver3[0] == p3[0]) and \
				(p3[1] is None or ver3[1] == p3[1]) and \
				(p3[2] is None or ver3[2] == p3[2]): return True
			elif t == '<':
				if ver3 < p3: return True
			else:
				print "unknown", pattern, ver

# 		errwrite(semver_diff(rc_semver.findone("1.1"), rc_semver.findone("1.11.0")))
# 		sys.exit()

		def relink(src, link, lvl):
			if islink(link) and not realpath(link).endswith(src):
				# tree(lvl, False, "unlink..", link, realpath(link), src)
				os.unlink(link)
			if not exists(link):
				try:
					os.symlink(src, link)
					return True
				except:
					errwrite("\nERR:symlink: %r" % [src, link, islink(link), realpath(link)])

		uniq_modules = {}
		downloads = []
		rebuilds = {}
		relinks = {}
		bin_links = {}
		walked = {}
		counter = [0, 0]  # all, installed

		def install_recur(fpath, lvl, prev_path, devDeps=False, flags=0):
			if islink(fpath) and exists(fpath):
				return

			pkg_file = join(fpath, "package.json")
			if not exists(pkg_file):
				# errwrite("ERROR: ", fpath, "no package.json")
				return

			with open(pkg_file, "rb") as src:
				pkg_json = cjson.decode(src.read())
				if not pkg_json:
					errwrite(fpath, "\tblank package.json")
					return

			pkgs, all_pkgs = {}, {}
			# install dependencies,peerDependencies by default
			if not args.no_prod:
				for t in ["dependencies", "peerDependencies"]:
					if t in pkg_json:
						pkgs.update(pkg_json[t])
						all_pkgs.update(pkg_json[t])

			# install devDependencies if specified (root package only)
			if devDeps and "devDependencies" in pkg_json:
				pkgs.update(pkg_json["devDependencies"])
				all_pkgs.update(pkg_json["devDependencies"])

			# don't install optionalDependencies
			if "optionalDependencies" in pkg_json:
				all_pkgs.update(pkg_json["optionalDependencies"])

			if not pkgs:
				# print fpath, "\tno deps"
				return

			if flags & 1:
				if lvl == 0:
					# print "\t" * lvl, fpath, pkgs
					errwrite(" " + pkg_json["name"])
			lvl += 1

			pkg_nm_path = join(fpath, "node_modules")
			if not exists(pkg_nm_path):
				try:
					os.mkdir(pkg_nm_path)
				except:
					errwrite("ERR:exists: %r" % [pkg_nm_path, exists(pkg_nm_path), fpath])

			for pair in pkgs.iteritems():
				k, v = pair
				if v and '/' not in v:
					vx = v.split("||")[-1].split(" - ")[-1].strip()
					if vx:
						if not args.semver_minor:
							if vx[0] in ['=', '~']:
								vx = '^' + vx[1:]
							elif vx[0].isdigit():
								vx = '^' + vx
						elif not args.semver_patch:
							if vx[0] == '=':
								vx = '~' + vx[1:]
							elif vx[0].isdigit():
								vx = '~' + vx
						# if v != vx:
						# 	print "\n", [v, vx], '\n'
						v = vx
						pair = k, v

				sub_pkg_path = join(pkg_nm_path, k)
				if pair in walked:
					tree(lvl, flags & 1, ("" if flags & 1 else "%d/%d:" % (counter[1], counter[0])), "%s/%s (%s) *" % (
						k, walked[pair].rsplit("/", 1)[-1], v))
					relink(walked[pair], sub_pkg_path, lvl)
					continue

				mod_base = join(args.pkg_dir, k)
				# print "\t" * lvl, mod_base

				if not exists(mod_base):
					if pair not in uniq_modules:
						uniq_modules[pair] = None
						downloads.append((k, v, mod_base, lvl, fpath))

					if not pair in relinks:
						relinks[pair] = []
					relinks[pair].append((sub_pkg_path, lvl))
					continue

				if exists(mod_base):
					vers = sorted([(rc_semver.findone(x), x)
						for x in listdir(mod_base)], reverse=True)
					ver = rc_semver.findone(v)
					vpath = None
					if ver is None or ver[1] is None:
						if vers:
							vx, vp = vers[0]
							vpath = join(mod_base, vp)
					else:
						for vx, vp in vers:
							if semver_diff(ver, vx):
								vpath = join(mod_base, vp)
								break

					if vpath is None:
						if pair not in uniq_modules:
							uniq_modules[pair] = None
							downloads.append((k, v, mod_base, lvl, fpath))

						if not pair in relinks:
							relinks[pair] = []
						relinks[pair].append((sub_pkg_path, lvl))
						continue

					if vpath and exists(vpath):
						counter[0] += 1
						tree(lvl, flags & 1, ("" if flags & 1 else "%d/%d:" % (counter[1], counter[0])), "%s/%s (%s)" % (
							k, vpath.rsplit("/", 1)[-1], v))
						relinked = relink(vpath, sub_pkg_path, lvl)
						walked[pair] = vpath
						if relinked or args.reinstall or flags & 2:
							install_recur(vpath, lvl, fpath, False, flags)
						counter[1] += 1
					else:
						errwrite("\t" * lvl, "module: %s (%s), version: %s not found, path: %s, %r, %r" % (k, mod_base, v, vpath, ver, vers))
				else:
					errwrite("\t" * lvl, "module: %s (%s) not found, version: %s" % (k, mod_base, v))

		def finalize():
			def worker_fn():
				while downloads:
					npm_pull(*downloads.pop())

			import threading
			errwrite("INFO:downloading and extracting new modules")
			while downloads:
				workers = []
				for _ in range(args.j):
					worker = threading.Thread(target=worker_fn)
					worker.start()
					workers.append(worker)
					if not downloads: break
				for worker in workers:
					worker.join()
			errwrite("INFO:done downloading")

			if not args.g:
				nm_path = join(args.base, 'node_modules')
				if not exists(nm_path):
					os.mkdir(nm_path)

			# relinks all sub packages
			errwrite("INFO:relinking modules")
			for pair, v in relinks.iteritems():
				for sub_pkg_path, lvl in v:
					# if pair[0] == 'semver':
					# 	errwrite('INFO:relink:', pair, uniq_modules[pair], sub_pkg_path, lvl)
					if uniq_modules[pair]:
						relink(uniq_modules[pair], sub_pkg_path, lvl)
						# errwrite(uniq_modules[pair], sub_pkg_path, lvl)
					else:
						errwrite("ERR:relink:", pair, uniq_modules[pair], sub_pkg_path, lvl)
			errwrite("INFO:done relinking")

			# rebuild all native modules
			errwrite("INFO:running scripts")
			builds = rebuilds.items()
			# print rebuilds
			def rebuild_fn():
				while builds:
					pair, (pkg_path, pkg_cmds) = builds.pop()
					for cmd in pkg_cmds:
						subprocess.call("cd %s && npm run %s" % (pkg_path, cmd), shell=True)

			while builds:
				workers = []
				for _ in range(8):
					worker = threading.Thread(target=rebuild_fn)
					worker.start()
					workers.append(worker)
					if not builds: break
				for worker in workers:
					worker.join()
			errwrite("INFO:done running scripts")

		def mk_binlinks(pkg_file, bin_base, mod_path=None):
			if not exists(pkg_file): print "404", pkg_file; return
			with open(pkg_file, "rb") as src:
				pkg_json = cjson.decode(src.read())
				if not pkg_json or "bin" not in pkg_json: return
				if type(pkg_json["bin"]) is str:
					bin_obj = {pkg_json["name"]: pkg_json["bin"]}
				else:
					bin_obj = pkg_json["bin"]
				for bin_name, fpath in bin_obj.iteritems():
					bin_path = join(bin_base, name)
					if mod_path:
						src_path = join(mod_path, fpath.replace("./", ""))
						print bin_path, src_path
						if not exists(bin_path) or args.reinstall:
							subprocess.call((sudo + "ln -snf '{0}' '{1}' && " + sudo + "chmod +x '{0}'").format(src_path, bin_path), shell=True)
					else:
						src_path = join('..', name, fpath.replace("./", ""))
						print bin_path, src_path
						if islink(bin_path) and not realpath(bin_path).endswith(src_path):
							os.unlink(bin_path)
						if not exists(bin_path):
							os.symlink(src_path, bin_path)
							os.chmod(bin_path, 0o755)

		if not args.g:
			root_pkg_file = join(args.base, 'package.json')
			nm_path = join(args.base, "node_modules")
			if not exists(nm_path):
				os.mkdir(nm_path)
			bin_base = join(nm_path, ".bin")
			if not exists(bin_base):
				os.mkdir(bin_base)

		if cmdn:
			# install specific modules
			for cmd in cmdn:
				mod_m, mod_v = cmd.split("@", 1) if "@" in cmd else (cmd, "*")

				errwrite('INFO:installing', mod_m, mod_v)
				npm_pull(mod_m, mod_v, join(args.pkg_dir, mod_m), 0, args.base)
				finalize()

				if args.g:
					print uniq_modules
					#{('bower', '*'): '/var/tmp/npmln/bower/1.8.0'}
					for (name, ver), mod_path in uniq_modules.iteritems():
						pkg_file = join(mod_path, 'package.json')
						mk_binlinks(pkg_file, join(args.prefix, 'bin'), mod_path)
				else:
					mk_binlinks(root_pkg_file, bin_base)

				# bin links global module
				#pair = mod_m, mod_v
				#if pair in uniq_modules:
				#	mod_g = uniq_modules[pair]

				#	if args.g:
				#		for mod_path, items in bin_links.iteritems():
				#			for prev_path, mod_name, bin_name, src_rel in items:
				#				if mod_path == mod_g:
				#					src_path = join(mod_path, src_rel)
				#					bin_path = join(args.prefix, "bin", bin_name)
				#					# print "global", src_path, bin_path
				#					if not exists(bin_path) or args.reinstall:
				#						subprocess.call((sudo + "ln -snf '{0}' '{1}' && " + sudo + "chmod +x '{0}'").format(src_path, bin_path), shell=True)
				#					# break
				#	else:
				#		nm_pkg_path = join(nm_path, mod_m)
				#		print mod_g, nm_pkg_path, mod_m, args.base
				#		if not exists(nm_pkg_path):
				#			os.symlink(mod_g, nm_pkg_path)
				## print "uniq", uniq_modules[(mod_m, mod_v)]
		else:
			install_recur(args.base, 0, args.base, args.dev)
			finalize()

			# clear the line
			sys.stdout.write("\r" + " " * __line_size + "\r")

			#make bin links
			with open(root_pkg_file, "rb") as root_src:
				root_pkg_json = cjson.decode(root_src.read())
				if root_pkg_json:
					pkgs = {}
					if not args.no_prod:
						pkgs.update(root_pkg_json.get('dependencies', {}))
					if args.dev:
						pkgs.update(root_pkg_json.get('devDependencies', {}))
					for name, v in pkgs.iteritems():
						pkg_file = join('node_modules', name, 'package.json')
						mk_binlinks(pkg_file, bin_base)

			# walk again to print tree, not needed for functionality
			install_recur(args.base, 0, args.base, args.dev, 1)

			if args.dump:
				walked.clear()
				install_recur(args.base, 0, args.base, args.dev, 2)
				with open(join(args.base, 'npmln.list'), 'wb') as dst:
					dst.write('\n'.join(sorted(relpath(x, args.pkg_dir) for x in set(walked.values()))))

	else:
		parser.print_help()
		print "Unknown command '%s'" % cmd0

if __name__ == '__main__':
	main()

