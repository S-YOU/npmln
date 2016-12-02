## NPMLN - Symlink NPM Modules (Experimental)

#### install npmln
```
sudo pip install npmln
```

#### usage
```bash
npmln i                                  # install dependencies and peerDependencies (./package.json)
npmln i --dev                            # install all including devDependencies (./package.json)
npmln i npm gulp grunt-cli typescript -g # install specific modules globally
npmln i bower --allow-node-modules -g    # install and allow builtin node_modules
```

#### supported formats in package.json are:
- name: version
- gzipped tarball url(http.\*?tar.gz)
- github urls (http.\*?.git, http.\*.git#branch)
- github (username/repo) version string
- npm's scoped modules (@scope/repo)

```
positional arguments:
  commands              npmln commands (install)

optional arguments:
  -h, --help            show this help message and exit
  -g                    symlink bin globally into (--prefix), with sudo or use
                        --no-sudo
  --no-sudo             don't use sudo, when you are root inside docker
                        (False)
  --base BASE           base directory for install (.), lookup package.json
  --prefix PREFIX       global bin scripts will be installed into
                        (/usr/local)/bin/
  --pkg-dir PKG_DIR     Package directory to be installed (/var/tmp/npmln)
  --tmp-dir TMP_DIR     tmp folder to download gzipped tarballs (/dev/shm)
  --no-prod             don't install dependencies and peerDependencies
                        (False)
  --dev                 install devDependencies (False)
  --semver-patch        respect semver patch (False)
  --semver-minor        respect semver minor (False)
  --retries RETRIES     retries when url read failed (20)
  -j J                  number of threads (64)
  --reinstall           reinstall specified module(s) regardless of link
                        exists (False)
  --allow-node-modules  allow node_modules folder to be included in package
                        (False)
  --no-cache            no cache for downloaded tarballs (False)
  --cache-dir CACHE_DIR
                        cache folder for tarballs (/var/tmp/npmln-cache)
  --dump                create list of install files (False)
  -q                    quiet (False)
  -v                    show program's version number and exit
```

#### installation requirements
- gcc
- python2-dev

#### run-time requirements
- `curl` - to download gzipped tarballs from github or npm registry
- `tar` with `--strip-components` options, normally GNU Tar, not busybox tar, to extract tar.gz file

#### Known issues
- does not work properly with package bundlers/transpilers/linters/module loaders/plugins loaders,
that does not use normal require('...') and don't have sub packages in dependencies section of package.json,
due to the nature of symlink, path resolving of those cannot be fixed.
- jquery/jquery
	- `npmln --base node_modules/grunt-eslint/node_modules/eslint/ i eslint-config-jquery`
