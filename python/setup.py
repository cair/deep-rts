from distutils.core import setup
from Cython.Build import cythonize
from Cython.Distutils import Extension
import os

os.environ["CC"] = "mingw64"
os.environ["CC"] = "g++-4.7"
os.environ["CXX"] = "g++-4.7"
extensions = [
    Extension("*", ["./**/*.pyx"], extra_compile_args=['-O3'])
]
setup(
    ext_modules = cythonize(extensions)
)