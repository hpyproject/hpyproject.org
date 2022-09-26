<!--
.. title: HPy Sprint Status Update and Feedback Session
.. slug: hpy_sprint_2022_report
.. date: 2022-09-26 10:00:00 UTC
.. author: fangerer
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

As we announced in
[this blog post](https://hpyproject.org/blog/posts/2022/07/dusseldorf-sprint-2022/),
we are currently having our joint PyPy/HPy/GraalPy sprint in Dusseldorf.

On Thursday (September 22nd, 2022), we did the HPy sprint report. Most notably,
we also had several CPython core developers in the call.

The sprint report was streamed on
[Twitch](https://www.twitch.tv/pypyproject) and the recording is available here:
<iframe width="560" height="315" src="https://www.youtube.com/embed/kl5w8uR6hGA" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<!--TEASER_END-->


## HPy Sprint Call with CPython Core Developers

One of the main goals in the sprint was to write a PEP to officially propose
HPy as an Python C extension API. We started to do so but before we actually
submit this PEP, we wanted to have some feedback. So, we invited the Python
core developers to our call and some of them joined.

We gave a short overview of HPy since not everyone might know it yet.
We talked about the goals and non-goals of HPy and basically asked if Python
core developers would be willing to participate in HPy design discussions.
We discussed PSF endorsement and the conclusion was that we need to talk to
the PSF.

We had some very good discussions and we also got some valuable feedback.
Our next steps are now to incorporate this feedback and prepare the PEP to
continue discussions there and gather more feedback.

We thank the Python core developers for participating in the call and for the
offered support.


## Call with NumPy Core Developer Sebastian Berg

Since we are working on the migration of NumPy to HPy, we had a call with
Sebastian Berg on Tuesday (September 20th, 2022).

We discussed some problems we discovered during our migration work and were
discussing how to solve it.

These were the main discussion points:

* NumPy's structured data type allows to have objects in NumPy arrays which
  problematic for HPy. This is mainly because doesn't allow to use the API
  during deallocation of an object. Hence, there is no way to determine the
  location of those objects by fetching the data type (which requires an API
  call). We discussed to mirror the structure in pure C-level data structures
  instead.

* HPy requires to migration all static types to heap types and we discovered
  that static type behave a bit differently compared to heap types in the case
  of multiple inheritance. We discussed if that is a bug that CPython needs
  to solve or if that is something that NumPy should change. So far, it looks
  rather to be a CPython problem since heap types are pretty new.

* We further discussed how to handle NumPy's C API when migrating to HPy. We
  agreed that we will probably need to provide a separate NumPy HPy C API as
  well which will maybe removed at some point in future.


## Main Technical Achievements of the Sprint

* We have added support for stack traces when running in the debug mode on Mac
  OS X (https://github.com/hpyproject/hpy/pull/346).

* We are up to finish our incremental porting example
  (https://github.com/hpyproject/hpy/pull/246/files)

* HPy does now also correctly support object members in HPy types
  (https://github.com/hpyproject/hpy/pull/347).
