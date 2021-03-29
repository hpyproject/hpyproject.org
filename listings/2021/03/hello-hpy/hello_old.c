#include <Python.h>

static PyObject* add(PyObject* self, PyObject* args) {
    long a, b;
    if (!PyArg_ParseTuple(args, "ll", &a, &b))
        return NULL;
    return PyLong_FromLong(a+b);
}

static PyMethodDef HelloMethods[] = {
    {"add", (PyCFunction)add, METH_VARARGS, "add two integers"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "hello_old",
    "hello example using the old Python/C API",
    -1,
    HelloMethods,
};

PyMODINIT_FUNC
PyInit_hello_old(void) {
    return PyModule_Create(&moduledef);
}
