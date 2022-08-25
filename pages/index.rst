.. title: HPy - A better C API for Python
.. slug: index
.. date: 2021-03-21 16:14:02 UTC
.. tags: 
.. category: 
.. link: 
.. description: 
.. type: text


What is HPy?
============

HPy provides a new API for extending Python in C.  In other words, you use
``#include <hpy.h>`` instead of ``#include <Python.h>``.

What are the advantages of HPy?
===============================

- **Zero overhead** on CPython: extensions written in HPy run at the same
  speed as "normal" extensions.

- **Much faster** on alternative implementations such as PyPy_, GraalPython_.

- **Universal binaries**: extensions built for the `HPy Universal ABI`_ can be
  loaded unmodified on CPython, PyPy, GraalPython, etc.

- **A migration path** for mixing legacy C-API calls with HPy API calls. Once
  all the code is migrated, the extension can be compiled as a universal binary
  that works on any CPython version, PyPy, or GraalPy.

- **Debug mode**: in `debug mode`_, you can easily identify common problems such
  as memory leaks, invalid lifetime of objects, invalid usage of APIs. Have
  you ever forgot a ``Py_INCREF`` or ``Py_DECREF``? The HPy debug mode can be
  activated at runtime to detect these mistakes for you on universal binaries.

- **Nicer API**: the standard Python/C API shows its age. HPy is designed to
  overcome some of its limitations, be more consistent, produce better quality
  extensions and to make it harder to introduce bugs.

- **Evolvability**: As nicely summarized in
  [PEP-620](https://peps.python.org/pep-0620/) the standard Python/C API
  exposes a lot of internal implementation details which makes it hard to
  evolve the C API. HPy doesn't have this problem because all internal
  implementation details are hidden.

.. _PyPy: https://pypy.org
.. _GraalPython:  https://www.graalvm.org/python
.. _`debug mode`: https://docs.hpyproject.org/en/latest/debug-mode.html
.. _`HPy Universal ABI`: https://docs.hpyproject.org/en/latest/overview.html#target-abis

Current status and where we need help
=====================================

HPy is in development. `0.0.4`__ is the latest alpha release.

.. __: https://hpyproject.org/blog/posts/2022/06/hpy-0.0.4-third-public-release/

We welcome your design input or adventurous alpha testing.

The Python/C API is huge. At the moment many popular functions are
available.  Is something missing for the port of your favorite extension?
Please open an issue or even better a PR.

Documentation could use improvement. Who better to suggest and implement
improvements than new users? The reference documentation lives together with
the code. We could also use help repurposing notes from the design discussions
and creating clearer documentation. If this non-coding work is something you
can do, let us know.

Tooling and packaging are no less difficult than managing low-level APIs. We
could use help driving discussion and implementing:

- How to package a universal extension and how to put it on PyPI?

- How best to package HPy itself

- How does HPy integrate with setuptools, mesonpy, or other build systems

- Upstreaming completed forks of the packages listed below, or upstreaming
  parts of the forks so the code does not suffer from bit-rot

HPy-compatible extensions
=========================

The extensions that we are experimenting with include:

- ultrajson-hpy_: this was the first real-world module to be ported to HPy. It is
  a nice fit because it only exports functions (as opposed as custom types)
  and requires only a small number of API functions.

- piconumpy_: as the name suggests, this is a minimal numpy-like module which
  defines a custom type (similar to ``ndarray`` but with many fewer features of
  course).

- numpy_: one of our ambitious goals is to port numpy to HPy, and to use this
  experience to better understand how to design the API. This builds but does
  not pass the test suite.

- matplotlib_: Since Matplotlib also has a dependency to NumPy, the migration
  to universal mode is not fully finished. HPy provides the legacy compatibility API
  such that we can still call legacy C API functions from HPy and successfully
  run the test suite.

- kiwi-solver_: A dependency of Matplotlib, it has been fully ported to
  universal mode.

Benchmarks and more information about the Matplotlib and kiwi-solver ports can
be found in the `GraalVM blog post`_. The benchmarks show that the HPy ports
have little impact on CPython performance, while enabling close-to-cpython
performance with GraalVM Python for the kiwi-solver complete HPy port.

.. _ultrajson-hpy: https://github.com/hpyproject/ultrajson-hpy
.. _piconumpy: https://github.com/hpyproject/piconumpy
.. _numpy: https://github.com/hpyproject/numpy-hpy/tree/graal-team/hpy#readme
.. _matplotlib: https://github.com/hpyproject/matplotlib-hpy/
.. _`kiwi-solver`: https://github.com/hpyproject/kiwi-hpy/
.. _`GraalVM blog post`: https://medium.com/graalvm/porting-matplotlib-from-c-api-to-hpy-aa32faa1f0b5

This website and the logo are ugly
===================================

We know: we have expertise in Virtual Machines, Compilers, low-level
programming, etc, but zero expertise in web development or graphic
design. `PRs are welcome ;) <https://github.com/hpyproject/hpyproject.org/>`_


More info
=========

- `Documentation`_

- `HPy blog`_

- `Github repository`_

- #hpy `IRC channel <irc://irc.libera.chat/hpy>`_

- `Mailing list`_

- HPy is pronounced `/h/ - pie` (or using IPA notation: `/eɪtʃ-paɪ/`)

.. _`Documentation`: https://hpy.readthedocs.io/en/latest/
.. _`HPy blog`: blog/
.. _`Github repository`: https://github.com/hpyproject/hpy/
.. _`Mailing list`: https://mail.python.org/archives/list/hpy-dev@python.org/latest

Recent blog posts
==================

.. post-list::
   :stop: 5
