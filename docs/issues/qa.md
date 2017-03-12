# Questions and Answers
This section of the documentation is a collection of known errors that may occur
during setup of the application:

**Error:**
```
Fatal Python error: Py_Initialize: unable to load the file system codec
ImportError: No module named 'encodings'
```

**Solution**: You must copy python3x.dll and python3x.gdb to the working directory
of your build. However this is not resolved for < Python 3.6