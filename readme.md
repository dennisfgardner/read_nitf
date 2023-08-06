# NITF Reader

Use the [NITRO-2.11.2](https://github.com/mdaus/nitro) to read a nitf image.

Here we build `<some_path>/nitro-NITRO-2.11.2/modules/c++/nitf/tests/test_extract.cpp`.

```bash
./build_script.sh
```

In the build script, for reasons I don't understand, it's important for `lsys-c++` to be listed before `str-c++`.
