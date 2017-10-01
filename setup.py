from distutils.msvc9compiler import VS_BASE

import sys
from distutils.core import setup, Extension
import shutil
import os
from Cython.Build import cythonize
import glob
from Cython.Distutils import build_ext


args = sys.argv[1:]


try:
    import shutil
    shutil.rmtree('build')
except FileNotFoundError as e:
    print("Nothing to clean!")


# We want to always use build_ext --inplace
if args.count("build_ext") == 0:
    sys.argv.append("build_ext")

if args.count("--inplace"):
    sys.argv.append("--inplace")

if os.name == "nt":
    sys.argv.append("--compiler=msvc")


files = ['cplus/src/bindings/python/Game.pyx']
for filename in glob.iglob('cplus/src/game/**/*.cpp', recursive=True):
    if "zmqAI" in filename:
        continue
    files.append(filename)


compile_args = ['-g', '-std=c++11']
basics_module = Extension('Game',
                          sources=files,
                          extra_compile_args=compile_args,
                          libraries=["sfml-window", "sfml-system", "sfml-graphics", "sfml-audio"],
                          language='c++')

setup(
    name='DeepRTS',
    packages=['DeepRTS'],
    cmdclass = {'build_ext': build_ext},
    ext_modules=cythonize(basics_module)
)


# Find compile file
build_file = list(glob.iglob('build/**/*.so', recursive=True))[0]
os.rename(build_file, 'build/' + os.path.basename(build_file))

shutil.copytree("cplus/src/game/data/", "build/data")