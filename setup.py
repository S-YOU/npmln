from setuptools import setup, Extension
import npmln

ext=Extension(
	'rc_semver',
	sources=['rc_semver.c'],
	extra_compile_args=['-O3','-std=c99','-Ofast','-Wall','-g0'],
	extra_link_args=['-s', '-g0'],
	language='c'
)

setup(
	name='npmln',
	author='S-YOU',
	author_email='S-YOU@users.noreply.github.com',
	version=npmln.__version__,
	description='Symlink NPM Modules',
	url='https://github.com/S-YOU/npmln',
	download_url='https://github.com/S-YOU/npmln/tarball/master',
	ext_modules=[ext],
	install_requires=['xxhash', 'python-cjson'],
	py_modules=['npmln'],
	entry_points={
		"console_scripts": [
			"npmln=npmln:main"
		]
	}
)
