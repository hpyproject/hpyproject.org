<!--
.. title: HPy @ Python Language Summit
.. slug: hpy-python-language-summit
.. date: 2021-05-12 10:00:00 UTC
.. author: antocuni
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

Yesterday I had the privilege to give a talk about HPy
([sildes](/talks/2021/05/hpy-present-and-future.pdf)) at the
[Python Language Summit 2021](https://us.pycon.org/2021/summits/language/).

The organizers of the summit will soon publish a full report about the event
(edit: now available
[here](https://pyfound.blogspot.com/2021/05/the-2021-python-language-summit.html)),
but [for the HPy-specific
part](https://pyfound.blogspot.com/2021/05/the-2021-python-language-summit-hpy.html),
we got generally good feedback. Someone has a few concerns that if CPython is
to change the API, HPy might not be going far enough. Others said that Python
shouldn't wait for the "perfect" API if HPy can be the "good" one that helps it
evolve.

Everyone was open to have HPy-compatible wheels on PyPI, once the HPy
Universal ABI stays relatively stable. Many people suggested that we should
really write a PEP to propose HPy as a "semi-official" API for Python.

<!--TEASER_END-->

An interesting question was about which are the VM optimizations which are
compatible with the HPy API. The following is a non-exhaustive list of things
which are known to work because they already used by PyPy and/or GraalPython:

- JIT compiler
- moving/compacting GCs
- storage strategies
- maps (also known as "hidden classes")

One notable missing optimization from the list above is tagged
pointers. Currently there is no implementation which uses tagged pointers and
supports HPy. However, we don't think there is any fundamental design issue in
HPy which would prevent it: if you turn tagged pointers into "tagged handles",
things should just work out of the box.
