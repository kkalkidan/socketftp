#include <python3.6/Python.h>


static PyObject *

 Socket(PyObject* self, PyObject *args){
    int result;
    result = create_socket();

    return Py_BuildValue("i", result);
}

static PyObject * 
server(PyObject* self, PyObject *args){
    int port;
    char* ip_addr;
    char* root_dir;
    int socket;
   if(!PyArg_ParseTuple(args, "iiss", &socket, &port, &ip_addr, &root_dir)){
        return NULL;
    }
    initialize_server(socket, port, ip_addr, root_dir);

    return Py_None;
}

static PyObject * 
client(PyObject* self, PyObject *args){
    int port;
    char* ip_addr;
    int socket;
   
    if(!PyArg_ParseTuple(args, "iis", &socket, &port, &ip_addr)){
        return NULL;
    }
    initialize_client(socket, port, ip_addr);

    return Py_None;
}

static PyMethodDef socketmethods[] = {
    {"socket", Socket, METH_VARARGS, "Create socket"},
    {"server", server, METH_VARARGS, "initialize server"},
    {"client", client, METH_VARARGS, "initialize client"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef Socketmodule = {
    PyModuleDef_HEAD_INIT,
    "socketftp",
    NULL,
    -1,
    socketmethods
};

PyMODINIT_FUNC PyInit_socketftp(void){
    return PyModule_Create(&Socketmodule);
}