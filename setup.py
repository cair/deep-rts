from distutils.core import setup
from Cython.Build import cythonize
from Cython.Distutils import Extension

extensions = [Extension("*", ["./**/*.pyx"])]
setup(
    ext_modules = cythonize(extensions)
)