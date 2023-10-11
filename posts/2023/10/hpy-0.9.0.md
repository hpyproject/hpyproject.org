<!--
.. title: hpy 0.9.0: Fourth public release
.. slug: hpy-0.9.0-fourth-public-release
.. date: 2023-10-12 19:00:00 UTC
.. author: fangerer
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

I'm excited to announce that HPy 0.9 was recently released!
The fourth official HPy release comes with many new features
and was again made available on PyPI.

This is the first release that also provides binary wheels, so installation is
now fast and smooth as you expect it.

The major highlights of the release are support subclassing of built-in types,
support for metaclasses, the trace mode, multi-phase module initialization, and
the hybrid ABI. Please checkout the full release notes here on GitHub:
https://github.com/hpyproject/hpy/releases/tag/0.9.0

Also, we invested a lot of effort to improve [official
documentation](https://docs.hpyproject.org/en/0.9.0/overview.html).

Great news too is that this release was used to make a first
(partially-migrated) port of NumPy, we call it
[NumPy/HPy](https://github.com/hpyproject/numpy-hpy), run some examples.

<!--TEASER_END-->

# What is HPy?

HPy provides a new API for extending Python in C. In other words, you use
`#include <hpy.h>` instead of `#include <Python.h>`. For more info, look at
the
[official documentation](https://docs.hpyproject.org/en/0.9.0/overview.html).

# Installation

HPy 0.9 is best tested on Linux and Mac OS X systems but there is also initial
support for Windows (both `x86_64`). For CPython, you need to install it
manually, using pip:

```shell
$ pip install hpy==0.9.0
```

[PyPy](https://pypy.org) and [GraalPy](https://graalvm.org/python/) usually
already come with intrinsic HPy support, so no installation is necessary.
HPy 0.9 is already included in [GraalPy
23.1](https://github.com/oracle/graalpython/releases/tag/graal-23.1.0).
Unfortunately, due to lack of resources, development is still in progress for
PyPy but should be finished soon.

Also checkout PyPy's nightly builds to follow the progress:

- [PyPy nightly builds](http://buildbot.pypy.org/nightly/)

To check the version of HPy which is shipped with those, you can either use
`pip` or `hpy.universal.get_version()`.

# Major Features

- Support subclasses of built-in types

It is now possible to create pure HPy types that inherit from built-in types like `type` or `float`. This was already possible before but in a very limited way, i.e., by setting `HPyType_Spec.basicsize` to `0`. In this case, the type implicitly inherited the basic size of the supertype but that also means that you cannot have a custom C struct. It is now possible inherit from a built-in type **AND** have a custom C struct. For further reference, see [HPyType_Spec.builtin_shape](https://docs.hpyproject.org/en/0.9.0/api-reference/hpy-type.html#c.HPyType_Spec.builtin_shape) and [HPyType_BuiltinShape](https://docs.hpyproject.org/en/0.9.0/api-reference/hpy-type.html#c.HPyType_BuiltinShape).

- Support for metaclasses

  HPy now supports creating types with metaclasses. This can be done by passing type specification parameter with kind `HPyType_SpecParam_Metaclass` when calling `HPyType_FromSpec`.

- [HPy Hybrid ABI](https://docs.hpyproject.org/en/0.9.0/overview.html#term-HPy-Hybrid-ABI)

  In addition to `CPython ABI` and `HPy Universal ABI`, we now introduced the Hybrid ABI. The major difference is that whenever you use a legacy API like `HPy_AsPyObject` or `HPy_FromPyObject`, the prdouced binary will then be specific to one interpreter. This was necessary to ensure that universal binaries are really portable and can be used on any HPy-capable interpreter.

- [Trace Mode](https://docs.hpyproject.org/en/0.9.0/trace-mode.html)

  Similar to the [Debug Mode](https://docs.hpyproject.org/en/0.9.0/debug-mode.html), HPy now provides the Trace Mode that can be enabled at runtime and helps analyzing API usage and identifying performance issues.

- [Multi-phase Module Initialization](https://docs.hpyproject.org/en/0.9.0/porting-guide.html#multi-phase-module-initialization)

  HPy now support multi-phase module initialization which is an important feature in particular needed for two important use cases: (1) module state support (which is planned to be introduced in the next major release), and (2) subinterpreters. We decided to drop support for single-phase module initialization since this makes the API cleaner and easier to use.

- HPy [Calling Protocol](https://docs.hpyproject.org/en/0.9.0/porting-guide.html#calling-protocol)

  This was a big missing piece and is now eventually available. It enables slot `HPy_tp_call`, which can now be used in the HPy type specification. We decided to use a calling convention similar to CPython's vectorcall calling convention. This is: the arguments are passed in a C array and the keyword argument names are provided as a Python tuple. Before this release, the only way to create a callable type was to set the special method `__call__`. However, this has several disadvantages. In particlar, poor performance on CPython (and maybe other implementations) and it was not possible to have specialized call function implementations per object (see `HPy_SetCallFunction`)
