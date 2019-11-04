import os
import re
import shutil
import sys
import platform
import subprocess

import os
dir_path = os.path.dirname(os.path.realpath(__file__))
assets_root = os.path.join(dir_path, "assets")
assets_py = os.path.join(dir_path, "DeepRTS", "python", "assets")

print(assets_root, assets_py)

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):

        extdir = os.path.join(os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name))), "DeepRTS")

        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPYTHON_EXECUTABLE=' + sys.executable,
            '-DPYTHON_BUILD=' + "true"
        ]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(['cmake', dir_path] + cmake_args, cwd=self.build_temp, env=env) # ext.sourcedir
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

try:
    shutil.copytree(assets_root, assets_py)
except:
    pass

setup(
    name='DeepRTS',
    version_format='{tag}.dev{commitcount}',  # +{gitsha}
    setup_requires=['setuptools-git-version', 'pybind11'],
    author='Per-Arne Andersen',
    author_email='per@sysx.no',
    url='https://github.com/cair/deep-rts',
    description='A Real-Time-Strategy game for Deep Learning research ',
    long_description='',
    include_package_data=True,
    packages=find_packages(
        exclude=["experiments", "docs", "include", "test"]
    ),
    ext_modules=[
        CMakeExtension('Engine')
    ],
    cmdclass=dict(
        build_ext=CMakeBuild,
    ),
    install_requires=['pygame', 'numpy', 'pillow'],
    zip_safe=False,
)
