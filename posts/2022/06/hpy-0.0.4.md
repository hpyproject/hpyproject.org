<!--
.. title: hpy 0.0.4: Third public release
.. slug: hpy-0.0.4-third-public-release
.. date: 2022-06-02 9:15:00 UTC
.. author: fangerer
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

HPy 0.0.4 is out! The third official HPy release comes with many new features
and was again made available on PyPI.

Major highlights of the release are a bunch of new API functions (e.g.
`HPyErr_ExceptionMatches`, `HPyErr_WarnEx`, `HPy_Contains`, and more),
Python 3.10 support, better support for native fields (`HPyField`) and global
variables (`HPyGlobal`), new debug mode features (detect invalid raw data
pointer usage, detect invalid closing of argument handles, detect return of
invalid handles).

Great news too is that we are now able to provide two more non-trivial projects
that have been (partially) migrated to HPy. This is,
[Kiwisolver](https://github.com/hpyproject/kiwi-hpy/)
and [Matplotlib](https://github.com/hpyproject/matplotlib-hpy/).

<!--TEASER_END-->

What is HPy?
============

HPy provides a new API for extending Python in C. In other words, you use
`#include <hpy.h>` instead of `#include <Python.h>`. For more info, look at
the
[official documentation](https://docs.hpyproject.org/en/0.0.4/overview.html).


Installation
============

HPy 0.0.4 is best tested on Linux systems but there is also initial support for
Windows (both `x86_64`).
For CPython, you need to install it manually, using pip:

```shell
$ pip install hpy==0.0.4
```


[PyPy](https://pypy.org) and [GraalPython](https://graalvm.org/python/) already
come with intrinsic HPy support, so no installation is necessary. HPy 0.0.4 will
be included in the next release of both. In the meantime, you can download a
nightly or dev build:

  - [PyPy nightly builds](http://buildbot.pypy.org/nightly/)

  - [GraalVM CE dev builds](https://github.com/graalvm/graalvm-ce-dev-builds/releases/)


To check the version of HPy which is shipped with those, you can either use
`pip` or `hpy.universal.get_version()`:

```shell
$ pypy -m pip show hpy
Name: hpy
Version: 0.0.4
...

$ graalpython -m pip show hpy
Name: hpy
Version: 0.0.4
...

$ pypy -c 'import hpy.universal; print(hpy.universal.get_version()[0])'
0.0.4

$ graalpython -c 'import hpy.universal; print(hpy.universal.get_version()[0])'
0.0.4
```

API
===

We are constantly working on the HPy API and keep adding functions that are
missing. We've added following API functions to the new release:

  - `HPyErr_SetFromErrnoWithFilename`, `HPyErr_SetFromErrnoWithFilenameObjects`
  - `HPyErr_ExceptionMatches`
  - `HPyErr_WarnEx`
  - `HPyErr_WriteUnraisable`
  - `HPy_Contains`
  - `HPyLong_AsVoidPtr`
  - `HPyLong_AsDouble`
  - `HPyUnicode_AsASCIIString`, `HPyUnicode_DecodeASCII`
  - `HPyUnicode_AsLatin1String`, `HPyUnicode_DecodeLatin1`
  - `HPyUnicode_DecodeFSDefault`, `HPyUnicode_DecodeFSDefaultAndSize`
  - `HPyUnicode_ReadChar`

For an overview of the current API, please refer to the public API declaration
in [`public_api.h`](https://github.com/hpyproject/hpy/blob/0.0.4/hpy/tools/autogen/public_api.h#L116-L440),
which is used to autogenerate parts of the HPy code and is a reliable list of
all the supported functions. Also have a look at additional helpers in
[`inline_helpers.h`](https://github.com/hpyproject/hpy/blob/0.0.4/hpy/devel/include/hpy/inline_helpers.h).

!!! Warning

    The HPy API is still considered in alpha status and it's subject to change
    between versions.

Debug Mode
==========

We again improved HPy's debug mode and added following new features:

Enable Debug Mode via Environment Variable
------------------------------------------

The debug mode can now be enabled using environment variable `HPY_DEBUG`. It is
possible to enable the debug mode for all HPy extensions or it is also possible
to enable it just for certain extensions by enumerating them.

Example:

```shell
$ # enable debug mode for all HPy extensions
$ HPY_DEBUG=1 python3 my_application.py

$ # enable debug mode just for ujson_hpy and piconumpy_hpy
$ HPY_DEBUG=ujson_hpy,piconumpy_hpy python3 my_application.py
```

Detect Invalid Use of Raw Data Pointers
---------------------------------------

Some API functions return a raw data pointer from an object. For example:
```
const char* HPyUnicode_AsUTF8AndSize(HPyContext *ctx, HPy h, HPy_ssize_t *size)
```
returns a raw data pointer to the UTF8 representation of a Python unicode
object. HPy doesn't expose the internal representation of the unicode object, so
the Python implementation may use an arbitrary internal representation. This
means that the UTF8 representation is just temporarily created for this API call
and so the raw data must be released at some point. The contract here is that
the raw data pointer is valid as long as the corresponding handle is valid.

Example:

```c
#include <string.h>

static const char *s_hello_world = "Hello, World!";

static const char * foo(HPyContext *ctx)
{
    HPy h_unicode = HPyUnicode_FromString(ctx, s_hello_word);
    HPy_ssize_t size;
    const char *res = HPyUnicode_AsUTF8AndSize(ctx, h_unicode, &size);

    /* closing 'h_unicode' is, of course, correct */
    HPy_Close(ctx, h_unicode);

    /* raw data pointer 'res' may have become invalid when closing
       'h_unicode' */
    return res;
}

static int bar(HPyContext *ctx)
{
    const char *s = foo(ctx);

    /* accessing 's' will cause a fatal error in debug mode (on supported
    systems) */
    return strcmp(s, s_hello_world) == 0;
}
```


It is easy to forget about this resriction and if the raw data pointer is used
after the handle was closed, it may point to garbage. If the debug mode is
enabled, it will make the underlying memory inaccessible and every access to the
pointer will then cause a crash of the application. This is currently only
implemented for Linux systems. We use a different strategy on other systems and
fill the pointer with some marker bytes that make it easy to detect.

Detect Incorrect Closing of Argument Handles
--------------------------------------------

HPy functions that are called from Python receive handles that are owned by the
caller. This means that those handles must not be closed by the callee but it
is, of course, possible to erroneously call `HPy_Close` on them. For example:

```c
HPyDef_METH(foo, "foo", foo_impl, HPyFunc_O, .doc="closing argument")
static HPy foo_impl(HPyContext *ctx, HPy self, HPy arg)
{
    // error: 'arg' is owned by the caller
    HPy_Close(ctx, arg);
    return HPy_Dup(ctx, ctx->h_None);
}
```

Detect Invalid Handles Returned from Function
---------------------------------------------

A common problem when returning handles is that the author may easily forget to
create a new handle. The debug mode now detects situations like the following:

```c
HPyDef_METH(foo, "foo", foo_impl, HPyFunc_NOARGS, .doc="returns arg w/o dupping it")
static HPy foo_impl(HPyContext *ctx, HPy self)
{
    // should be: return HPy_Dup(ctx, self);
    return self;
}
```

Examples
========

Besides the known examples, this is HPy's 
["proof of concept" package](https://github.com/hpyproject/hpy/tree/0.0.4/proof-of-concept), 
[`ultrajson-hpy`](https://github.com/hpyproject/ultrajson-hpy/tree/hpy-0.0.4),
[`piconumpy`](https://github.com/hpyproject/piconumpy/tree/hpy-0.0.4), we are
excited to present two new packages we have migrated to HPy:

  - [Kiwi](https://github.com/hpyproject/kiwi-hpy/) 
    is an efficient C++ implementation of the Cassowary constraint solving
    algorithm.

  - [Matplotlib](https://github.com/hpyproject/matplotlib-hpy/)
    is a comprehensive library for creating static, animated, and interactive
    visualizations in Python.
    Since Matplotlib also has a dependency to NumPy, the migration is not fully
    finished but luckily, HPy provides the legacy compatibility API such that we
    can still call legacy C API functions from HPy.

We are still cleaning these ports up and will write another blog post about the
ports and open them for discussion with the project owners.
