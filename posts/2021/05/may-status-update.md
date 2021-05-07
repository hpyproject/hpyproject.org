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
of the world, HPy has come a long way since then. This is a quick update
to say where we are now and what's planned for the immediate future.

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
  - Compile an extension in either "universal" or "cpython" modes (see below)
  - Put a "universal" extension into debug mode **at runtime** (see below
    for how this is possible without a performance penalty).
  - Compile extensions for POSIX and Windows (MSC).

An extension written using HPy may be compiled either in "cpython" or
"universal" mode:

  - "cpython" mode produces an ordinary Python C extension that only uses
    the HPy API internally. From the point of view of the Python implementation
    it is an ordinary Python C extension.

  - "universal" mode produces an HPy C extension. This extension does not use
    the Python C API directly at all. A Python implementation must know how
    to load such an extension and pass it an HPy context. Currently context
    implementations exist for CPython, PyPy and GraalPython.

Debug mode is implemented by replacing the HPy context with a debug mode
context at runtime. The debug context wraps the original context and tracks
all HPy API calls. Debug mode already supports finding handles that are
leaked (i.e. not closed) and handles that are used after being closed.
Additional debug features should now be fairly straightforward to add.

The debug mode is written using HPy itself, so its implementation can be
shared by different Python implementations that support HPy.


What are the next steps?
========================

The big picture is that we're working towards writing a Cython backend
and porting core features of numpy to HPy. There is still some way to go
but the immediate next steps planned are:

  - Implement ``HPyField``. HPy handles are always short lived. Often it
    is necessary to store long-lived references to Python objects though.
    For example, a custom type may wish to store an array containing
    references to Python objects, or have attributes that reference them.
    These long-lived references will be stored as ``HPyField``s, and can
    be thought of as a second type of handle. HPy handles are for short-lived
    references. ``HPyFields`` are for long-lived references.

  - Implement module level state. These will be primarily used to
    store and access module constants and custom types from within the
    methods of the module itself and will be implemented using ``HPyField``s.

  - Implement multi-phase Python module initialization (HPy currently uses the
    legacy module initialization).

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
