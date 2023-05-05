<!--
.. title: Hello, HPy
.. slug: hello-hpy
.. date: 2021-03-29 10:00:00 UTC
.. author: antocuni
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

Welcome to the shiny new HPy blog!

HPy has been around for a while now. The initial discussion started during
EuroPython 2019, in the good old times when we could still go to conferences
and have real-life meetings. Since then, HPy progressed **a lot** from the
point of view of the actual code, but we have been a bit too silent
w.r.t. communicating what we are doing to the external world and to the broader
Python community. Hopefully, now that this blog is online we will do a better
job at periodically communicating the status of HPy, so make sure to
[subscribe to the RSS feed](/rss.xml).

<!--TEASER_END-->

# What is HPy?

Quoting the frontpage of our website:

> HPy provides a new API for extending Python in C. In other words, you use
> `#include <hpy.h>` instead of `#include <Python.h>`.

The official [Python/C API](https://docs.python.org/3/c-api/index.html>) is
specific to the current implementation of CPython: it exposes many
internal details which makes it hard:

- to implement it for other Python implementations (e.g. PyPy, GraalPython,
  Jython, IronPython, etc.)

- to experiment with new things inside CPython itself: e.g. using a GC
  instead of refcounting, or to remove the GIL.
- to correctly check things like refcount handling: the external API gets mixed in with implementation details that should be hidden.

Over the years, it has become evident that
[emulating the Python/C API in an efficient way is challenging](https://www.pypy.org/posts/2018/09/inside-cpyext-why-emulating-cpython-c-8083064623681286567.html),
if not impossible. The main goal of HPy is expose a C API which is possible
to implement in an efficient way on a number of very diverse Python
implementations.

There are several advantages to writing your C extension in HPy:

- it runs much faster on PyPy, and at native speed on CPython

- it is possible to compile a single binary which runs unmodified on all
  supported Python implementations and versions

- it is simpler and more manageable than the Python/C API

- it provides an improved debugging experience: in "debug mode", HPy
  actively checks for many common mistakes such as reference leaks and
  invalid usage of objects after they have been deleted. It is possible to
  turn the "debug mode" on at startup time, without needing to recompile
  Python or the extension itself

See also the official docs for a more in-depth
[overview](https://docs.hpyproject.org/en/latest/overview.html#hpy-overview).

# Show me an example

This is a "normal" Python/C extension:

{{% listing 2021/03/hello-hpy/hello_old.c c %}}

We can compile it with a simple `setup.py`:

```python
from setuptools import setup, Extension
setup(
    name="hello",
    ext_modules = [
        Extension('hello_old', ['hello_old.c']),
    ],
)
```

```pycon
$ python setup.py build_ext --inplace
...
$ python
>>> import hello_old
>>> hello_old.add(10, 20)
30
```

Now, let's start to uncover the HPy equivalent, piece by piece:

```c
#include <hpy.h>

HPyDef_METH(add, "add", add_impl, HPyFunc_VARARGS,
            .doc = "add two integers");

static HPy add_impl(HPyContext ctx, HPy self, HPy *args, HPy_ssize_t nargs) {
    long a, b;
    if (!HPyArg_Parse(ctx, NULL, args, nargs, "ll", &a, &b))
        return HPy_NULL;
    return HPyLong_FromLong(ctx, a+b);
}
```

There are a bunch of things which are different from the usual C-extension module:

- the former `PyObject *` is now `HPy`, which we call "a handle". Handles are
  similar to `PyObject *`, but are completely opaque: for more information,
  see the
  [official docs](https://docs.hpyproject.org/en/latest/api.html#handles).

- There is an additional parameter, `HPyContext ctx`. One of the problems of
  the old API is that often it implicitly relies on the existence of a
  per-thread or per-subinterpreter local state. `HPyContext` makes this state explicit. This
  makes the whole API more regular and makes it possible to develop new
  interesting features such as the
  [Universal ABI](https://docs.hpyproject.org/en/latest/overview.html#term-HPy-Universal-ABI)
  and the [Debug mode](https://github.com/hpyproject/hpy/pull/142).

- HPy introduces the concept of `HPyDef`s. `HPyDef_METH` is a macro which
  generates the definition of an `HPyDef` static constant named `add`, which
  represents the definition of a Python method implemented by the C function
  `add_impl`. In this specific example `HPyDef_METH` contains more or less the
  same informations as the old `PyMethodDef`, but `HPyDef` are more
  general. For example, when defining custom types you can use things like
  `HPyDef_SLOT`, `HPyDef_GETSET`, etc.

- Notice that we no longer need the cast to `(PyCFunction)`. One of the
  biggest advantages of `HPyDef_METH` is that since it's a macro, it can
  automatically generate a forward declaration for `add_impl`, with the
  correct signature. This means that if you use the wrong number and/or type
  of parameters, you get a nice compile-time error instead of an obscure crash
  at runtime.

- The signature corresponding to `HPyFunc_VARARGS` is slighly different than
  the old `METH_VARARGS`: we pass positional arguments as a C array instead of
  a Python tuple. This means that it is possible to call the function without
  having to allocate a Python tuple, and for example the PyPy implementation
  of HPy takes advantage of that. This is very similar to CPython's
  [VectorCall protocol](https://www.python.org/dev/peps/pep-0590/).

!!! Note

    In this post, we are using a slightly old version of HPy. If you try with
    a newer version you should use `HPyContext *ctx` instead of `HPyContext ctx`.
    See also [Issue #150](https://github.com/hpyproject/hpy/issues/150)
    and [PR #182](https://github.com/hpyproject/hpy/pull/182).

Let's continue our tour of `hello_new.c`:

```c
static HPyDef *hello_defines[] = {
    &add,
    NULL
};

static HPyModuleDef moduledef = {
    HPyModuleDef_HEAD_INIT,
    .m_name = "hello_new",
    .m_doc = "hello example using the new HPy API",
    .m_size = -1,
    .defines = hello_defines,
};

HPy_MODINIT(hello_new)
static HPy init_hello_new_impl(HPyContext ctx) {
    return HPyModule_Create(ctx, &moduledef);
}
```

This is pretty similar to the old code. The biggest change is that instead of
declaring an array of `PyMethodDef`, we create an array of `HPyDef` as
discussed above.

Finally, we need to modify `setup.py`. Compiling an HPy extension is as easy
as adding `setup_requires=['hpy.devel']` and use `hpy_ext_modules`:

{{% listing 2021/03/hello-hpy/setup.py python %}}

# Compiling HPy extensions

In this demo, we will show how to setup an environment to try HPy and compile
extensions on both CPython and PyPy.

At the moment HPy is still in its early stages and the API is still subject to
change, so we have not done any official release yet. For the same reason, if
you want to use HPy on PyPy or GraalPython, you need to manually ensure to install
a version which is supported. This is just temporary, and this kind of things will
be sorted out automatically once we start to roll out official releases.

So, we need to install HPy from the github repo. Moreover, the HPy
implementations inside PyPy and GraalPython are lagging behind a little, so we will
install a slightly old revision:

- [HPy revision eb07982](https://github.com/hpyproject/hpy/commit/eb07982)

- nightly build of the PyPy hpy branch: [a2f7c80062e8 for linux64](http://buildbot.pypy.org/nightly/hpy/pypy-c-jit-101860-a2f7c80062e8-linux64.tar.bz2)

- PyPy nighly builds: [main page](http://buildbot.pypy.org/nightly/) and
  [hpy branch](http://buildbot.pypy.org/nightly/hpy/)
- GraalPython nightly build: [Linux](https://github.com/graalvm/graalvm-ce-dev-builds/releases/download/21.1.0-dev-20210330_0726/graalpython-dev-linux-amd64.tar.gz) or [macOS](https://github.com/graalvm/graalvm-ce-dev-builds/releases/download/21.1.0-dev-20210330_0726/graalpython-dev-macos-amd64.tar.gz)

- [source code](https://github.com/hpyproject/hpyproject.org/tree/main/listings/2021/03/hello-hpy) of this example

The first step is to create a `venv` for CPython and install `hpy`:

```shell
$ python3 -m venv tryhpy
$ . tryhpy/bin/activate
$ pip install wheel
$ pip install git+git://github.com/hpyproject/hpy.git@eb07982
```

To install a nightly build of PyPy it is enough to unpack the tarball and run
`-m ensurepip`. We can check what is the HPy version supported by PyPy by
calling `hpy.universal.get_version()`:

```shell
$ curl -O http://buildbot.pypy.org/nightly/hpy/pypy-c-jit-101860-a2f7c80062e8-linux64.tar.bz2
$ tar xf pypy-c-jit-101860-a2f7c80062e8-linux64.tar.bz2
$ ./pypy-c-jit-101860-a2f7c80062e8-linux64/bin/pypy -m ensurepip

$ ./pypy-c-jit-101860-a2f7c80062e8-linux64/bin/pypy
>>>> import hpy.universal
>>>> hpy.universal.get_version()
('0.1.dev959+geb07982', 'eb07982')
```

For GraalPython, just unpack the tarball and create a venv:

```shell
$ curl -LO https://github.com/graalvm/graalvm-ce-dev-builds/releases/download/21.1.0-dev-20210330_0726/graalpython-dev-linux-amd64.tar.gz
$ tar xzf graalpython-dev-linux-amd64.tar.gz
$ graalpython-21.1.0-dev-linux-amd64/bin/graalpython -m venv hpy-venv
$ hpy-venv/bin/graalpython
>>> import hpy.universal
>>> hpy.universal.get_version()
>>> ('0.1.dev950+g98f448a', '98f448a')
```

Now that our enviroment is ready, we can compile and try our extensions:

```shell
$ cd /path/to/example/
$ . /path/to/tryhpy/bin/activate    # activate the venv
$ python setup.py build_ext --inplace
[...]

$ ls -1 *.so
hello_new.cpython-38-x86_64-linux-gnu.so
hello_old.cpython-38-x86_64-linux-gnu.so

$ python
>>> import hello_old, hello_new
>>> hello_old.add(10, 20)
30
>>> hello_new.add(30, 40)
70
>>>
```

It worked! One important thing to note is the filename of `hello_new`:
`.cpython-38-x86_64-linux-gnu.so` is the standard filename for CPython 3.8
extension modules. This happens because by default `hpy_ext_modules` targets
the
[CPython ABI](https://docs.hpyproject.org/en/latest/overview.html#target-abis). As
such, from the point of view of CPython `hello_new` is indistinguishable from
`hello_old`. It also means that HPy is required only to compile it but not to
import it later. Finally, we expect the performance to be the very same as the
extensions using the old API.

However, we can also explicitly ask HPy to produce an "universal binary",
which targets the
[HPy Universal ABI](https://docs.hpyproject.org/en/latest/overview.html#target-abis):
as the name implies, universal binaries can be imported by CPython, but also
by alternative implementations such as PyPy. We can build universal binaries
by passing `--hpy-abi=universal` to `setup.py`:

```shell
$ # clean the previous build
$ rm -rf build/ *.so

$ python setup.py --hpy-abi=universal build_ext --inplace
$ ls -1 *.so
hello_new.hpy.so
hello_old.cpython-38-x86_64-linux-gnu.so
```

Note the filename: `hello_old` is still a CPython-specific extension, but
`hello_new.hpy.so` is an universal binary. Once compiled, you can import it as
usual:

```pycon
$ python
>>> import hello_old, hello_new
>>> hello_old.add(10, 20)
30
>>> hello_new.add(30, 40)
70
>>> hello_new.__file__
'/.../hello-hpy/hello_new.hpy.so'
```

!!! note

    At the moment of writing, because of
    [Issue #191](https://github.com/hpyproject/hpy/issues/191) if you try to
    print the repr of `hello_new`, you see something like this (note the `.py`
    extension):

        >>> hello_new
        <module 'hello_new' from '/.../hello-hpy/hello_new.py'>

Note that on its own, CPython does **not** know how to import `.hpy.so`
files. The magic is done by the `hello_new.py`, which is automatically
generated by `setup.py`:

```python
$ cat hello_new.py
[...]
def __bootstrap__():
    [...]
    from hpy.universal import load_from_spec
    ext_filepath = pkg_resources.resource_filename(__name__, 'hello_new.hpy.so')
    m = load_from_spec(Spec('hello_new', ext_filepath))
    [...]
    sys.modules[__name__] = m

__bootstrap__()
```

Finally, we can try to import our shiny new universal binary on PyPy:

```shell
$ /path/to/pypy-c-jit-101860-a2f7c80062e8-linux64/bin/pypy
>>>> import hello_new
>>>> hello_new.add(10, 20)
30
>>>> hello_new.__file__
'/.../hello-hpy/hello_new.hpy.so'
>>>>
```

Similarly, it also just works on GraalPython:

```shell
$ /path/to/graalpython/hpy-venv/bin/graalpython
>>> import hello_new
>>> hello_new.add(10, 20)
30
```

That's all you need to get started with HPy. What we presented today is just
the basics, of course: in the next posts we will dig more into the technical
details, and show more interesting features than just a hello world.

Stay tuned!

_(edited on 2021-03-31 to include GraalPython)_
