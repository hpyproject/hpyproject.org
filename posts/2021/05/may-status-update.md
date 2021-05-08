<!--
.. title: HPy Status Update
.. slug: may-status-update
.. date: 2021-05-07 10:00:00 UTC
.. author: hodgestar
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->


HPy's first git commit was on Friday, 12 July 2019. Despite the crazy state
of the world, [HPy](https://github.com/hpyproject/hpy/) has come a long way
since then. This is a quick update to say where we are now and what's planned
for the immediate future.

<!--TEASER_END-->

What can one do with HPy right now?
===================================

One can already port non-trivial Python extensions to the HPy API, but
there is also still a lot of work to do. Things one can do right now
with HPy include:

  - Initialize an extension module
  - Create custom extension types
  - Create custom methods and functions
  - Interact with many built-in types (including dictionaries, tuples,
    lists, strings, bytes and longs)
  - Raise and check exceptions
  - Parse arguments to C functions
  - Call Python functions
  - Compile an extension against either the "universal" or "cpython" ABIs (see below)
  - Load a single compiled "universal" extension into different Python implementations.
  - Put a "universal" extension into debug mode **at runtime** (see below
    for how this is possible without a performance penalty).
  - Compile extensions for POSIX and Windows (MSVC).

An extension written using HPy may be compiled against either the "cpython" or
"universal" ABI:

  - compiling against the "cpython" ABI produces an ordinary Python C extension.
    The extension uses the HPy API only internally. From the point of view of
    the Python implementation the compiled module is an ordinary C extension
    that just happens to have been compiled using HPy. Note that as is the case
    for ordinary C extensions, the modules produced target a specific
    [ABI tag](https://www.python.org/dev/peps/pep-0425/).

  - compiling against the "universal" ABI produces an HPy C extension. This
    extension does not use the Python C API directly at all and the ABI
    is unique (up to platform and HPy version number). The same compiled module
    may thus be loaded by multiple Python versions and implementations.
    A Python implementation must know how to load such an extension and have
    implemented the HPy universal ABI. Universal ABI implementations exist for
    CPython, PyPy and GraalPython.

Debug mode is implemented by replacing the HPy context with a debug mode
context at runtime. The debug context wraps the original context and tracks
all HPy API calls. Debug mode already supports finding handles that are
leaked (i.e. not closed) and handles that are used after being closed.
Additional debug features will be fairly straightforward to add, like for example checking for `NULL` pointers or detecting whether the user is trying to call a function without holding the GIL.

The debug mode is written using HPy itself, so its implementation can be
shared by different Python implementations that support HPy.


What are the next steps?
========================

The big picture is that we're working towards writing a Cython backend
and porting core features of numpy to HPy. There is still some way to go
but the immediate next steps planned are:

  - Add support for storing long-lived references to other Python objects in
    custom types, i.e. the equivalent of storing a ``PyObject *`` in
    one of the fields with the struct of a custom type. HPy handles are only for
    short-lived references, so we need a GC-friendly way to store long-lived
    references.
    See [issue #9](https://github.com/hpyproject/hpy/issues/9).

  - Implement isolated module level state, to avoid using C static global
    variables. This will be the HPy equivalent of
    [PEP 573](https://www.python.org/dev/peps/pep-0573/) and
    [PEP 630](https://www.python.org/dev/peps/pep-0630/).

  - Implement multi-phase Python module initialization (HPy currently uses the
    legacy module initialization).
    See [issue #183](https://github.com/hpyproject/hpy/pull/184).

  - Update the PyPy and GraalPython HPy implementations to support all the
    latest HPy features.

  - Start work on an HPy backend for Cython.

  - Continuing work on a minimal port of numpy's array type to HPy.

  - Ask the CPython core developers whether we can find a way to support
    HPy universal extensions directly within in CPython (currently we write
    a small `.py` file that knows how to load the universal extension, but
    the presence of this stub file causes many small edge cases that are
    annoying for HPy developers and users)

If you'd like to work on any of these with us, drop us a note in the
comments or mailing list or join us on IRC.

Happy hacking,
The HPy Team.
