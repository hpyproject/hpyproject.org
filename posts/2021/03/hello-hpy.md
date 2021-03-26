<!--
.. title: Hello, HPy
.. slug: hello-hpy
.. date: 2021-03-26 16:00:00 UTC
.. author: Antonio Cuni
.. tags: 
.. category: 
.. link: 
.. description: 
.. type: text
-->

Welcome to the shiny new HPy blog!

HPy has been around for a while now. The initial discussion started during
EuroPython 2019, at the good old times when we could still go to conferences
and have real-life meetings. Since then, HPy progressed **a lot** from the
point of view of the actual code, but we have been a bit too silent
w.r.t. communicating what we are doing to the external world and to the broader
Python community. Hopefully, now that this blog is online we will do a better
job at periodically communicating the status of HPy, so make sure to
[subscribe to the RSS feed](/rss.xml).

What is HPy? Show me an example
================================

Quoting the frontpage of our website:

> HPy provides a new API for extending Python in C. In other words, you use
> `#include <hpy.h>` instead of `#include <Python.h>`.

But admittedly this is a bit obscure, better to show some code to make things
clearer. 

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

static HPy add_impl(HPyContext *ctx, HPy self, HPy *args, HPy_ssize_t nargs) {
    long a, b;
    if (!HPyArg_Parse(ctx, NULL, args, nargs, "ll", &a, &b))
        return HPy_NULL;
    return HPyLong_FromLong(ctx, a+b);
}
```

There are a bunch of things which immediately pop to the eyes:

- the former `PyObject *` is now `HPy`, which we call "a handle". Handles are
  similar to `PyObject *`, but are completely opaque: for more information,
  see the
  [official docs](https://docs.hpyproject.org/en/latest/api.html#handles).

- There is an additional parameter, `HPyContext *ctx`. One of the problems of
  the old API is that often it implicitly relies on the existence of a
  per-thread or per-subinterpreter local state. `HPyContext` make it explicit,
  makes the whole API more regular and make it possible to develop new
  interesting features such as the
  [Universal ABI](https://docs.hpyproject.org/en/latest/overview.html#term-HPy-Universal-ABI)
  and the (Debug mode)[https://github.com/hpyproject/hpy/pull/142].

- HPy introduces the concept of `HPyDef`s. `HPyDef_METH` is a macro which
  generates the definition of an `HPyDef` global variable named `add`, which
  is a method implemented by the C function `add_impl`. In this specific
  example `HPyDef_METH` contains more or less the same informations as the old
  `PyMethodDef`, but `HPyDef` are more general. For example, when defining
  custom types you can use things like `HPyDef_SLOT`, `HPyDef_GETSET`, etc.

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

Let's continue our tour of `hello_new.c`:

```c
static HPyDef *hello_defines[] = {
    &add,
    NULL
};

static HPyModuleDef moduledef = {
    HPyModuleDef_HEAD_INIT,
    .m_name = "hello_old",
    .m_doc = "hello example using the new HPy API",
    .m_size = -1,
    .defines = hello_defines,
};

HPy_MODINIT(hello_new)
static HPy init_hello_new_impl(HPyContext *ctx) {
    return HPyModule_Create(ctx, &moduledef);
}
```

This is pretty similar to the old code. The biggest change is that instead of
declaring an rray of `PyMethodDef`, we create an array of `HPyDef` as
discussed above.

Finally, we need to modify `setup.py`: compiling an HPy extension is as easy
as adding `setup_requires=['hpy.devel']` and use `hpy_ext_modules`:

{{% listing 2021/03/hello-hpy/setup.py python %}}

