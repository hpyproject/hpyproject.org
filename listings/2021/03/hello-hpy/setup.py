from setuptools import setup, Extension
setup(
    name="hello",
    ext_modules = [
        Extension('hello_old', ['hello_old.c']),
    ],
    hpy_ext_modules = [
        Extension('hello_new', ['hello_new.c']),
    ],
    setup_requires=['hpy.devel'],
)
