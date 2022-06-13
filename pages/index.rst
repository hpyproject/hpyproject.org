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

- **Debug Mode**: in debug mode, you can easily identify common problems such
  as memory leaks, invalid lifetime of objects, invalid usage of APIs. Have
  you ever forgot a ``Py_INCREF`` or ``Py_DECREF``? The HPy debug mode will
  detect these mistakes for you.

- **Universal binaries**: extensions built for the *HPy Universal ABI* can be
  loaded unmodified on CPython, PyPy, GraalPython, etc.

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

Current status
==============

HPy is still in an early stage of development and there are only alpha preview
releases so far, `0.0.4`__ being the most recent one.

.. __: https://hpyproject.org/blog/posts/2022/06/hpy-0.0.4-third-public-release/

The Python/C API is huge and at the moment only a small fraction of it is
available in HPy. We are experimenting by porting existing extensions to HPy
and adding/designing new functionalities on a per-need basis.

We welcome your design input or adventurous alpha testing.

HPy-compatible extensions
=========================

The extensions that we are experimenting with include:

- ultrajson-hpy_: this was the first real-world module to be ported to HPy. It is
  a nice fit because it only exports functions (as opposed as custom types)
  and requires only a small number of API functions.

- piconumpy_: as the name suggests, this is a minimal numpy-like module which
  defines a custom type (similar to ``ndarray`` but with many fewer features of
  course).

- numpy-hpy_: one of our ambitious goals is to port numpy to HPy, and to use this
  experience to better understand how to design the API. This is still at a
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

- #hpy IRC channel on the `Libera.Chat`_ server: `irc.libera.chat/hpy <irc://irc.libera.chat/hpy>`_

  * **NOTE**: the official #hpy channel used to be on Freenode, `but it is no
    longer the case`_

- HPy is pronounced `/h/ - pie` (or using IPA notation: `/eɪtʃ-paɪ/`)

.. _`Official docs`: https://hpy.readthedocs.io/en/latest/
.. _`HPy blog`: blog/
.. _`Github repository`: https://github.com/hpyproject/hpy/
.. _`Mailing list`: https://mail.python.org/mailman3/lists/hpy-dev.python.org/
.. _`but it is no longer the case`: /blog/posts/2021/05/hpy-irc-moves-to-libera-chat/
.. _`Libera.Chat`: https://libera.chat/

Recent blog posts
==================

.. post-list::
   :stop: 5
