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

    static char* kwlist[] = {"dict_path", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &dict_path)) {
        return -1;
    }

    if (!PyString_Check(dict_path)) {
        PyErr_SetString(PyExc_TypeError,
                        "The dict path must be a string");
        return -1;
    }

    char* _dict_path = PyString_AsString(dict_path);
    self->ip_lib->LoadDict(_dict_path);
    return 0;
}

static PyMemberDef IP2Location_members[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef IP2Location_getseters[] = {
    {NULL} /* Sentinel */
};

static PyMethodDef IP2Location_methods[] = {
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
    0, //IP2Location_methods,             /* tp_methods */
    0, //IP2Location_members,             /* tp_members */
    0, //IP2Location_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)IP2Location_init,      /* tp_init */
    0,                         /* tp_alloc */
    IP2Location_new,                 /* tp_new */
};

typedef struct {
    PyObject_HEAD
    int number;
} Noddy;

static void
Noddy_dealloc(Noddy* self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Noddy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Noddy *self;
    self = (Noddy *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int
Noddy_init(Noddy *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

static PyMemberDef Noddy_members[] = {
    {"number", T_INT, offsetof(Noddy, number), 0, "noddy number"},
    {NULL}  /* Sentinel */
};

static PyMethodDef Noddy_methods[] = {
    {NULL}  /* Sentinel */
};

static PyTypeObject NoddyType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "ip_lib.Noddy",             /*tp_name*/
    sizeof(Noddy),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)Noddy_dealloc, /*tp_dealloc*/
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
    "Noddy objects",           /* tp_doc */
    0,                     /* tp_traverse */
    0,                     /* tp_clear */
    0,                     /* tp_richcompare */
    0,                     /* tp_weaklistoffset */
    0,                     /* tp_iter */
    0,                     /* tp_iternext */
    Noddy_methods,             /* tp_methods */
    Noddy_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Noddy_init,      /* tp_init */
    0,                         /* tp_alloc */
    Noddy_new,                 /* tp_new */
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
    if (m == NULL) {
        return;
    }
//    PyModule_AddObject(m, "IP2Location", (PyObject*)&IP2LocationType);
    PyModule_AddObject(m, "Noddy", (PyObject*)&NoddyType);
}

