import pathlib
import re

import sys
import platform
import subprocess
import os

from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        if not self._check_cmake():
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        for ext in self.extensions:
            self.build_extension(ext)

    @staticmethod
    def _check_cmake():
        try:
            out = subprocess.check_output(['cmake', '--version'])
            if platform.system() == "Windows":
                cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
                if cmake_version < '3.1.0':
                    raise RuntimeError("CMake >= 3.1.0 is required on Windows")
            return True
        except OSError:
            return False

    def build_dependencies(self, current_dir: pathlib.Path, build_dir: pathlib.Path):
        # Get the absolute path of the current directory

        # VC pkg dir = Current directory + /vcpkg
        vcpkg_config_dir = current_dir.resolve()

        vcpkg_build_dir = (build_dir / "vcpkg").resolve()
        toolchain_file = vcpkg_build_dir / "scripts" / "buildsystems" / "vcpkg.cmake"

        # Get macOS version and set MACOSX_DEPLOYMENT_TARGET environment variable
        # macos_version = subprocess.check_output(["sw_vers", "-productVersion"]).decode().strip().rsplit('.', 1)[0]
        # os.environ["MACOSX_DEPLOYMENT_TARGET"] = macos_version

        # Install dependencies
        subprocess.run([
            str(vcpkg_config_dir / "vcpkg_install.sh"),
            str(build_dir.absolute())
        ], check=True)

        return toolchain_file

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.join(os.path.dirname(self.get_ext_fullpath(ext.name)), "DeepRTS"))
        cfg = 'Debug' if self.debug else 'Release'

        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DPython_EXECUTABLE=' + sys.executable,
            '-DPYTHON_BUILD=true'
        ]

        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2 ** 32:
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

        # Current dir
        current_dir = pathlib.Path.cwd()

        # Build dependencies
        toolchain_file = self.build_dependencies(current_dir, pathlib.Path(self.build_temp))

        # CMake
        subprocess.run([
            "cmake",
            # "-GNinja",
            f"-B{self.build_temp}",
            "-S.",
            f"-DCMAKE_TOOLCHAIN_FILE={toolchain_file}",
            "-DCMAKE_BUILD_TYPE=Release"
        ] + cmake_args,
            check=True,
            cwd=current_dir,
            env=env
        )

        # CMake build
        subprocess.run([
            "cmake",
            "--build",
            self.build_temp
        ] + build_args,
            check=True,
            cwd=current_dir
        )


setup(
    name='DeepRTS',
    author='Per-Arne Andersen',
    author_email='per@sysx.no',
    url='https://github.com/cair/deep-rts',
    description='A Real-Time-Strategy game for Deep Learning research ',
    long_description='',
    include_package_data=True,
    packages=find_packages(
        exclude=["coding"],
        include=["DeepRTS*"]

    ),
    ext_modules=[
        CMakeExtension('libdeeprts')
    ],
    cmdclass=dict(
        build_ext=CMakeBuild,
    ),
    install_requires=['numpy'],
    zip_safe=False,
)
