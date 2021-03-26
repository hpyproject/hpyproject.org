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
