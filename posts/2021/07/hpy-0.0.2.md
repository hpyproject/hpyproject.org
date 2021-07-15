<!--
.. title: hpy 0.0.2: First public release
.. slug: hpy-0.0.2-first-public-release
.. date: 2021-07-19 10:00:00 UTC
.. author: antocuni
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

hpy 0.0.2 is out! This is the first version which is officially released and
made available on PyPI.

The major highlight of this release is that it is supported by three different
Python implementations: CPython, PyPy and GraalPython.

<!--TEASER_END-->

What is HPy?
============

HPy provides a new API for extending Python in C. In other words, you use
`#include <hpy.h>` instead of `#include <Python.h>`. For more info, look at
the
[official documentation](https://docs.hpyproject.org/en/latest/overview.html).


Installation
============

For CPython, you need to install it manually, using pip:

```
$ pip install hpy==0.0.2
```

PyPy and GraalPython ships their own version of HPy, so no installation is
necessary. HPy 0.0.2 will be included in the next release of both, i.e. PyPy
x.x.x (expected on 2021-xx-xx) and GraalPython y.y.y (expected on
2021-yy-yy). In the meantime, you can download a nightly build:

  - [PyPy 3.7 nightly builds](http://buildbot.pypy.org/nightly/py3.7/)

  - GraalPython: download the latest `graalpython-dev` package from
    [this page](https://github.com/graalvm/graalvm-ce-dev-builds/releases)

To double check the version of HPy which is shipped with those, you can either
use `pip` or `hpy.universal.get_version()`:

```
$ pypy -m pip show hpy
Name: hpy
Version: 0.0.2
Summary: A better C API for Python
Home-page: https://hpyproject.org
Author: The HPy team
Author-email: hpy-dev@python.org
License: MIT
...

$ pypy -c 'import hpy.universal; print(hpy.universal.get_version()[0])'
0.0.2
```

API
===

At the moment HPy supports only a small fraction of the full API offered by
the old Python/C API, but it is enough to write non-trivial extensions, and
the documentation is
scarce. [`public_api.h`](https://github.com/hpyproject/hpy/blob/release/0.0.2/hpy/tools/autogen/public_api.h#L107-L288),
which is used to autogenerate parts of the HPy code, is a reliable list of all
the supported functions.

!!! Warning

    The HPy API is still considered in alpha status and it's subject to change
    between versions. In fact, the current master is already incompatible with
    hpy-0.0.2 because of
    [PR #182](https://github.com/hpyproject/hpy/pull/182), which renamed all
    occurences of `HPyContext` into `HPyContext*`.



Examples
========

The best way to get a glimpse of how to use HPy is to look at examples:

  - the HPy repository contains a
    ["proof of concept" package](https://github.com/hpyproject/hpy/tree/release/0.0.2/proof-of-concept). 
    Make sure to checkout the branch `release/0.0.2`.

  - [`ultrajson-hpy`](https://github.com/hpyproject/ultrajson-hpy/tree/hpy-0.0.2)
    is a port of the popular `ultrajson` package. Make sure to checkout the
    `hpy-0.0.2` branch.

  - [`piconumpy`](https://github.com/hpyproject/piconumpy/tree/hpy-0.0.2)
    contains a very tiny implementation of an `array`-like class. Make sure to
    checkout the `hpy-0.0.2` branch.
