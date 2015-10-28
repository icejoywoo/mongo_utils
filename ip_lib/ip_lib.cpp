#include "Python.h"
#include "structmember.h"

#include <string>
#include "ip_table.h"


IPLib ip_lib;

static PyObject* load_dict_function(PyObject *self, PyObject *args) {
    PyObject* dict_path = NULL;
    if (!PyArg_ParseTuple(args, "O", &dict_path)) {
        return NULL;
    }

    if (!PyString_Check(dict_path)) {
        PyErr_SetString(PyExc_TypeError,
                        "The dict path must be a string");
        Py_INCREF(Py_False);
        return Py_False;
    }

    char* _dict_path = PyString_AsString(dict_path);
    ip_lib.LoadDict(_dict_path);

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject* get_country_function(PyObject *self, PyObject *args) {
    PyObject* ip = NULL;
    if (!PyArg_ParseTuple(args, "O", &ip)) {
        return NULL;
    }

    if (!PyString_Check(ip)) {
        PyErr_SetString(PyExc_TypeError,
                        "The ip must be a string");
        Py_INCREF(Py_False);
        return Py_False;
    }

    std::string country;
    const char* _ip = PyString_AsString(ip);
    ip_lib.GetCountry(_ip, country);

    return Py_BuildValue("s", country.c_str());
}

static PyObject* cleanup_function(PyObject *self, PyObject *args) {
    ip_lib.CleanUp();
    Py_INCREF(Py_True);
    return Py_True;
}

static PyMethodDef ip_lib_methods[] = {
    {"load_dict", (PyCFunction)load_dict_function, METH_VARARGS, "load dict to memory"},
    {"get_country", (PyCFunction)get_country_function, METH_VARARGS, "get country by ip"},
    {"cleanup", (PyCFunction)cleanup_function, METH_VARARGS, "cleanup memory"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initip_lib(void) {
    PyObject* m;
    m = Py_InitModule3("ip_lib", ip_lib_methods, "ip lib");
}

