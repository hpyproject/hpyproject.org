from setuptools import setup, Extension
setup(
    name="hello",
    ext_modules = [
        Extension('hello_old', ['hello_old.c']),
    ],
)
