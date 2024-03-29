<!--
.. title: HPy meetup and CPython core dev sprint in Brno (Oct 7-10, 2023)
.. slug: hpy-meetup-2023
.. date: 2023-10-10 10:00:00 UTC
.. author: fangerer
.. tags:
.. category:
.. link:
.. description:
.. type: text
-->

We just ended our rather informal HPy meetup in Brno (Czech Republic) which
started on Saturday (October 7th) and lasted until Tuesday (October 10th).

The main topics of the HPy meetup were:

- The CPython Core Dev Sprint
- Cython/HPy

<!--TEASER_END-->

# The CPython Core Dev Sprint

Petr Viktorin, one of the CPython core developers, invited us to
partially attend the CPython core developer sprint. As a part of the sprint, he
organized a C API Summit Monday where we were invited and happily attended this
event.

Besides of lots of discussions, there were also very interesting presentations
about the C API:

## 1. C API Work Group Summary

Irit
[presented](https://github.com/iritkatriel/talks/blob/main/2023_Sprint_Brno_C_API.pdf)
the current status of the [C API Work Group GitHub](https://github.com/capi-workgroup)
organisation. The idea of this GitHub org was to gather curent
[problems](https://github.com/capi-workgroup/problems) of the C API and also to
collect ideas for the (re-)volution of the API. Irit thankfully went over all
issues and discussions to structure and summarize them. She identified nine
categories for problems:

1. API Evolution and Maintenance
2. API Specification and Abstraction
3. Object Reference Management
4. Type Definition and Object Creation
5. Error Handling
6. API Tiers and Stability Guarantees
7. The C Language
8. Implementation Flaws
9. Missing Functionality

She also mentioned strengths of the C API which the people contributing to the repository
agree on:

- Heap types
- APIs taking a C string to look up based on Python strings
- Limited API and Stable ABI

Now, the idea is to write an _Informational PEP_ and then to create:

1. A process for discussions about replacement APIs
2. Guidelines for [evolution of the C API](https://github.com/capi-workgroup/api-evolution).

The [PEP draft](https://github.com/python/peps/pull/3491) is already available.

## 2. Petr's Vision for the C API

Petr presented his understanding of how the C API should look in future. The
[slides](https://drive.google.com/file/d/148NLRPXGZGI1SXfKLMFvQc_iv67hPJQS/view?usp=sharing)
for his talk are public. He made seven suggestions:

1. _Split_ API from ABI so you can compile the API that it would use different ABI
   variants.

2. _Consistency_: The API should have a consistent behavior. E.g. APIs should
   not return borrowed references and the exception behavior should be clear by the
   naming convention.

3. _Versioning_: Essentially, a C extension should include the ABI version it
   was compiled with.

4. _Errors_: All API functions must be able to signal an error that can be
   handled.

5. _Concrete types_: The API should provide dedicated C types for certain
   built-in objects to have type safety already at the C language level.

6. _Blueprint structures_: Creating types and objects should rather be done by
   using specifications than by allocating empty objects and filling them with
   content. Just think of `PyType_Spec` or `PyModuleDef`.

7. _Context argument_: Every API function should have a context argument.

Except for points 4 and 5, these goals are very close to HPy's. We, the
HPy team, have to say that this sounds very good and we would be happy to see
that in CPython.

## 3. HPy's Lessons Learned

We were also asked to give a [talk](/talks/2023/10/things_to_learn_from_hpy.pdf)
and to concentrate in the talk on two things:

1. How do handles affect the API, and
2. What is the reason for the HPy context to contain the function table.

We didn't _advertise_ HPy in our talk but we talked about why opaque handles
are not only useful but sometimes necessary and how the API would change
compared to having only `PyObject *`.
We further showed that having the function table in the context gives us a lot
of flexibility and finally enables us to switch on the debug mode essentially at
run time (when an extension is loaded).

We are not sure if we managed to convince everyone that those concepts are useful.
The biggest problem is that introducing handles and a context argument will
radically change the API and require changes in many libraries (see also
[C API Workgroup - API Revolution #2](https://github.com/capi-workgroup/api-revolution/issues/2)
and [C API Workgroup - API Revolution #7](https://github.com/capi-workgroup/api-revolution/issues/7)).

## 4. Python C API: Past, Present, and Future

Victor did a thorough [presentation](https://github.com/vstinner/talks/blob/main/2023-CoreDevSprint-Brno/c-api.pdf)
of the past, present, and (possible) future of the Python C API. He pointed out
some past and present problems and gave some impression of its complexity. Victor
also suggests to make the stable ABI be the default. Overall, he sees a
positive trend in how the C API evolves.

# Discussions

We had a lot of good discussions and probably the most important one was about
the [PEP 703 (noGIL)](https://peps.python.org/pep-0703/) implementation and the
new version of ABI (abi4) that is planned to be compatible with both noGIL and
GIL builds of CPython. In order to achieve that, Sam, the author of PEP
703, plans to make `PyObject *` opaque (so it will narrow to the semantics of a
handle) and also, the `Py_INCREF` will, in some cases, use an indirection over a
function pointer to actually call the incref logic. This is, again, a big step
forward for alternative implementations.

# Cython/HPy

Besides the main focus on the C API Summit at the CPython Core Dev Sprint, we
were also hacking a bit on Cython/HPy and made the doctest system work.
The problem there was that the doctest module is binding functions to self in
order to get a method but HPy did not yet provide the necessary slot
`HPy_tp_descr_get` for that. So, we've implemented that and will include it in
the next (minor) release.

We hoped that we would do more hacking on HPy itself but the CPython core dev
sprint was worth to spend our time too.
