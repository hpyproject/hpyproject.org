<!--
.. title: hpy 0.0.3: Second public release
.. slug: hpy-0.0.3-second-public-release
.. date: 2021-10-22 15:00:00 UTC
.. author: fangerer
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

HPy 0.0.3 is out! This is already HPy's second officially released version and
was again made available on PyPI.

The major highlights of this release are new API functions, new debug mode
features, and Windows support.

<!--TEASER_END-->

# What is HPy?

HPy provides a new API for extending Python in C. In other words, you use
`#include <hpy.h>` instead of `#include <Python.h>`. For more info, look at
the
[official documentation](https://docs.hpyproject.org/en/latest/overview.html).

# Installation

HPy 0.0.3 is best tested on Linux systems but there is also initial support for
Windows (both `x86_64`).
For CPython, you need to install it manually, using pip:

```
$ pip install hpy==0.0.3
```

!!! Note

    Currently, we provide only the `sdist` (i.e., the `.tar.gz`, no binary
    wheels). But we plan to ship binary wheels starting with the next release.

[PyPy](https://pypy.org) and [GraalPython](https://graalvm.org/python/) already
come with intrinsic HPy support, so no installation is necessary. HPy 0.0.3 is
included in the current releases of both, i.e. PyPy 7.3.6 (released on
2021-10-17) and GraalPython 21.3.0 (released on 2021-10-19).

To double check the version of HPy which is shipped with those, you can either
use `pip` or `hpy.universal.get_version()`:

```
$ pypy -m pip show hpy
Name: hpy
Version: 0.0.3
...

$ graalpython -m pip show hpy
Name: hpy
Version: 0.0.3
...

$ pypy -c 'import hpy.universal; print(hpy.universal.get_version()[0])'
0.0.3

$ graalpython -c 'import hpy.universal; print(hpy.universal.get_version()[0])'
0.0.3
```

# API

HPy is still under heavy development and does not yet provide as many API
functions as Python's C API, but more and more pieces are being added.
In this release, we've added following functions:

- `HPy_Is`
- `HPyBytes_FromStringAndSize`
- `HPyErr_NewException`
- `HPyErr_NewExceptionWithDoc`
- `HPyUnicode_AsUTF8AndSize`
- `HPyUnicode_DecodeFSDefault`
- `HPyImport_ImportModule`

All of the above functions except of `HPy_Is` have equivalents in the C API. You
just need to remove the leading `H` from the name.

Function `HPy_Is` was added to be able to do object identity comparisons. In the
C API, you would just do a pointer comparison like this:

```
PyObject *object_a = /* ... */;
PyObject *object_b = /* ... */;
return object_a == object_b;
```

In HPy, direct comparisons of handles are forbidden by design, because distinct
handles can point to the same object. To compare objects by identity, you need
to use `HPy_Is`:

```
HPy object_a = /* ... */;
HPy object_b = /* ... */;
return HPy_Is(ctx, object_a, object_b);
```

We've also extended functions `HPyArg_Parse` and `HPyArg_ParseKeywords` and
added support for format specifier `s`. The new specifier converts a Python
unicode object into a C string. For example:

```
const char *result;
if (!HPyArg_Parse(ctx, NULL, args, nargs, "s", &result)) {
    return HPy_NULL;
}
printf("result = %s\n", result);
```

For an overview of the current API, please refer to the public API declaration
in [`public_api.h`](https://github.com/hpyproject/hpy/blob/release/0.0.3/hpy/tools/autogen/public_api.h#L107-L298),
which is used to autogenerate parts of the HPy code and is a reliable list of
all the supported functions.

!!! Warning

    The HPy API is still considered in alpha status and it's subject to change
    between versions.

# Debug Mode

One of the great features of HPy is the _debug mode_. It was already introduced
a while ago (and therefore already included in release 0.0.2) but it got a new
feature that can help C extension developers a lot. HPy's debug mode is now able
to track closed handles. Previously, it was not guaranteed that accesses to
closed handles will cause a fatal error. This is because it's possible that
another object is associated with the closed handle. Then the program would just
operate on the wrong object. The new feature is able to prevent that.

# Examples

The best way to get a glimpse of how to use HPy is to look at examples:

- the HPy repository contains a
  ["proof of concept" package](https://github.com/hpyproject/hpy/tree/release/0.0.3/proof-of-concept).
  Make sure to checkout the branch `release/0.0.3`.

- [`ultrajson-hpy`](https://github.com/hpyproject/ultrajson-hpy/tree/hpy-0.0.3)
  is a port of the popular `ultrajson` package. Make sure to checkout the
  `hpy-0.0.3` branch.

- [`piconumpy`](https://github.com/hpyproject/piconumpy/tree/hpy-0.0.3)
  contains a very tiny implementation of an `array`-like class. Make sure to
  checkout the `hpy-0.0.3` branch.
