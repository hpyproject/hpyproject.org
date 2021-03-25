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
