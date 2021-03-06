#include "Python.h"
#include "structmember.h"

#include <string>
#include "ip_table.h"

// IP2Location type

typedef struct {
    PyObject_HEAD
    IPLib* ip_lib;
} IP2Location;

static void
IP2Location_dealloc(IP2Location* self) {
    delete self->ip_lib;
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
IP2Location_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    IP2Location* self;
    self = (IP2Location *)type->tp_alloc(type, 0);
    self->ip_lib = new IPLib();
    return (PyObject*) self;
}

static int
IP2Location_init(IP2Location* self, PyObject* args, PyObject* kwds) {
    PyObject* dict_path = NULL;
    PyObject* callback = NULL;

    static char* kwlist[] = {"dict_path", "callback", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|O", kwlist, &dict_path, &callback)) {
        return -1;
    }

    if (!PyString_Check(dict_path)) {
        PyErr_SetString(PyExc_TypeError,
                        "The dict path must be a string");
        return -1;
    }

    if (callback) {
        if (!PyCallable_Check(callback)) {
            PyErr_SetString(PyExc_TypeError,
                            "The callback must be a function");
            return -1;
        }
        char* _dict_path = PyString_AsString(dict_path);
        FILE* f = fopen(_dict_path, "r");
        char buf[1024];
        PyObject* arglist;
        PyObject* result;

        PyObject* start;
        PyObject* end;
        PyObject* country;
        while (fgets(buf, sizeof(buf), f)) {
            arglist = Py_BuildValue("(s)", buf);
            result = PyObject_CallObject(callback, arglist);
            Py_DECREF(arglist);
            if (PyArg_ParseTuple(result, "OOO", &start, &end, &country)) {
                char* _start = PyString_AsString(start);
                char* _end = PyString_AsString(end);
                char* _country = PyString_AsString(country);
                self->ip_lib->PushItem(_start, _end, _country);
            }
            Py_DECREF(result);
        }
    } else {
        char* _dict_path = PyString_AsString(dict_path);
        self->ip_lib->LoadDict(_dict_path);
    }

    return 0;
}

static PyMemberDef IP2Location_members[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef IP2Location_getseters[] = {
    {NULL} /* Sentinel */
};

static PyObject *
IP2Location_get_country(IP2Location* self, PyObject* args)
{
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
    self->ip_lib->GetCountry(_ip, country);

    if (country.empty()) {
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        return Py_BuildValue("s", country.c_str());
    }
}

static PyObject *
IP2Location_cleanup(IP2Location* self)
{
    self->ip_lib->CleanUp();
    Py_INCREF(Py_True);
    return Py_True;
}

static PyMethodDef IP2Location_methods[] = {
    {"get_country", (PyCFunction)IP2Location_get_country, METH_VARARGS, "get country by ip"},
    {"cleanup", (PyCFunction)IP2Location_cleanup, METH_NOARGS, "cleanup"},
    {NULL}  /* Sentinel */
};

static PyTypeObject IP2LocationType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "ip_lib.IP2Location",             /*tp_name*/
    sizeof(IP2Location),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)IP2Location_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "IP2Location objects",           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    IP2Location_methods,             /* tp_methods */
    IP2Location_members,             /* tp_members */
    IP2Location_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)IP2Location_init,      /* tp_init */
    0,                         /* tp_alloc */
    IP2Location_new,                 /* tp_new */
};

// module functions

static IPLib ip_lib;

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

    if (country.empty()) {
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        return Py_BuildValue("s", country.c_str());
    }
}

static PyObject* cleanup_function(PyObject *self) {
    ip_lib.CleanUp();
    Py_INCREF(Py_True);
    return Py_True;
}

static PyMethodDef ip_lib_methods[] = {
    {"load_dict", (PyCFunction)load_dict_function, METH_VARARGS, "load dict to memory"},
    {"get_country", (PyCFunction)get_country_function, METH_VARARGS, "get country by ip"},
    {"cleanup", (PyCFunction)cleanup_function, METH_NOARGS, "cleanup memory"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initip_lib(void) {
    PyObject* m;
    if (PyType_Ready(&IP2LocationType) < 0) {
        return;
    }

    m = Py_InitModule3("ip_lib", ip_lib_methods, "ip lib");
    if (m == NULL) {
        return;
    }
    PyModule_AddObject(m, "IP2Location", (PyObject*)&IP2LocationType);
}

