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

---

**Error:**
When running out of diskspace, your application will break on if you have flatbuffers serialization active
```py
uint8_t *make_space(size_t len) {
    if (len > static_cast<size_t>(cur_ - buf_)) {
      reallocate(len);
    }
    cur_ -= len;
    // Beyond this, signed offsets may not have enough range:
    // (FlatBuffers > 2GB not supported).
    assert(size() < FLATBUFFERS_MAX_BUFFER_SIZE);
    return cur_;
}
```

**Solution**: Deactivate flatbuffer serialization or free disk space