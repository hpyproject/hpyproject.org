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

- **Zero overhead** on CPython: extensions written in HPy runs at the same
  speed as "normal" extensions.

- **Much faster** on alternative implementations such as PyPy_, GraalPython_.

- **Debug Mode**: in debug mode, you can easily identify common problems such
  as memory leaks, invalid lifetime of objects, invalid usage of APIs. Have
  you ever forgot a ``Py_INCREF`` or ``Py_DECREF``? The HPy debug mode will
  detect this mistakes for you.

- **Universal binaries**: extensions built for the *HPy Universal ABI* can be
  loaded unmodified on CPython, PyPy, GraalPython, etc.

- **Nicer API**: the standard Python/C API shows its age. HPy is designed to
  overcome some of its limitations, be more consistent, produce better quality
  extensions and to make it harder to introduce bugs.

.. _PyPy: https://pypy.org
.. _GraalPython:  https://www.graalvm.org/python

Current status
==============

HPy is still in an early stage of development and there is no official release
yet. We hope to make our first alpha release in the next months but we don't
have any ETA.

The Python/C API is huge and at the moment only a small fraction of it is
available in HPy. We are experimenting by porting existing extensions to HPy
and adding/designing new functionalities on a per-need basis.

We welcome your design input or adventurous alpha testers.

HPy-compatible extensions
==========================

The extensions that we are experimenting with include:

- ultrajson-hpy_: this was the first real-world module to be ported to HPy. It is
  a nice fit because it only exports functions (as opposed as custom types)
  and require only a small number of API functions.

- piconumpy_: as the name suggests, this is a minimal numpy-like module which
  defines a custom type (similar to ``ndarray`` but with much less features of
  course).

- numpy-hpy_: one of our ambitious goals is to port numpy to HPy, and to use this
  experience to better understand how to design the API. This is still is a
  **very** early stage though.

.. _ultrajson-hpy: https://github.com/hpyproject/ultrajson-hpy
.. _piconumpy: https://github.com/hpyproject/piconumpy
.. _numpy-hpy: https://github.com/hpyproject/numpy-hpy/

This website and the logo are ugly
===================================

We know: we have expertise in Virtual Machines, Compilers, low-level
programming, etc, but zero expertise in web development or graphic
design. `PRs are welcome ;) <https://github.com/hpyproject/hpyproject.org/>`_


More info
=========

- `Official docs`_

- `HPy blog`_

- `Github repository`_

- #hpy IRC channel on the Freenode_ server

.. _`Official docs`: : https://hpy.readthedocs.io/en/latest/
.. _`HPy blog`: blog/
.. _`Github repository`: https://github.com/hpyproject/hpy/
.. _`Mailing list`: https://mail.python.org/mailman3/lists/hpy-dev.python.org/
.. _Freenode: https://freenode.net/

Recent blog posts
==================

.. post-list::
   :stop: 5
