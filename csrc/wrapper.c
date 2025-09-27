#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "vector.h"
#include "matrix.h"

static PyObject* py_create_vec(PyObject* self, PyObject* args){
    PyObject* py_list;
    // Parse Python args 
    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_list)){
        return NULL;
    }
    int size = (int)PyList_Size(py_list);
    if(size <= 0){
        PyErr_SetString(PyExc_ValueError, "List must have at least one element");
        return NULL;
    }
    // Create the c vector
    Vector* v = init_vec(size);
    // copy python floats into c array
    for(int i=0;i<size;i++){
        PyObject* item = PyList_GetItem(py_list, i);
        if (!PyFloat_Check(item) && !PyLong_Check(item)) {
            free_vec(v);
            PyErr_SetString(PyExc_TypeError, "List elements must be numbers");
            return NULL;
        }
        v->data[i] = (float)PyFloat_AsDouble(item);
    }
    // Wrap the pointer in a PyCapsule to return to Python
    return PyCapsule_New((void*)v, "Vector", NULL);
}

static PyObject* py_free_vec(PyObject* self, PyObject* args){
    PyObject* capsule;
    // parse argument: expect a a PyCapsule
    if(!PyArg_ParseTuple(args, "O", &capsule)){
        return NULL;
    }   
    // Extract Vector* from capsule
    Vector* v = (Vector*)PyCapsule_GetPointer(capsule,"Vector");
    if(v==NULL){
        return NULL;
    }

    //free c mem
    free_vec(v);
    //PyCapsule_SetPointer(capsule, NULL);
    Py_RETURN_NONE;
}

static PyObject* py_print_vec(PyObject* self, PyObject* args){
    PyObject* capsule;
    if(!PyArg_ParseTuple(args, "O", &capsule)){
        return NULL;
    }
    Vector* v = (Vector*)PyCapsule_GetPointer(capsule,"Vector");
    if(v==NULL){
        return NULL;
    }

    print_vec(v);
    Py_RETURN_NONE;
}

static PyObject* py_vec_add(PyObject* self, PyObject* args){
    PyObject *capsule1, *capsule2;
    // Parse arguments: two PyCapsules
    if(!PyArg_ParseTuple(args, "OO", &capsule1, &capsule2)){
        return NULL;
    }

    Vector *v1 = (Vector*)PyCapsule_GetPointer(capsule1, "Vector");
    Vector *v2 = (Vector*)PyCapsule_GetPointer(capsule2,"Vector");

    if(!v1 || !v2){
        return NULL;
    }

    Vector* res = vec_add(v1, v2);
    return PyCapsule_New((void*)res, "Vector", NULL);
}

static PyObject* py_vec_sub(PyObject* self, PyObject* args){
    PyObject *capsule1, *capsule2;
    // Parse arguments: two PyCapsules
    if(!PyArg_ParseTuple(args, "OO", &capsule1, &capsule2)){
        return NULL;
    }

    Vector *v1 = (Vector*)PyCapsule_GetPointer(capsule1, "Vector");
    Vector *v2 = (Vector*)PyCapsule_GetPointer(capsule2,"Vector");

    if(!v1 || !v2){
        return NULL;
    }

    Vector* res = vec_sub(v1, v2);
    return PyCapsule_New((void*)res, "Vector", NULL);
}

static PyObject* py_scalar_mul(PyObject* self, PyObject* args){
    PyObject *capsule;
    float scalar;

    if (!PyArg_ParseTuple(args, "Of", &capsule, &scalar)) return NULL;

    Vector *v = (Vector*)PyCapsule_GetPointer(capsule, "Vector");
    if (!v) return NULL;

    Vector *result = scalar_mul(v, scalar);

    return PyCapsule_New((void*)result, "Vector", NULL);
}

static PyObject* py_scalar_div(PyObject* self, PyObject* args){
    PyObject *capsule;
    float scalar;

    if (!PyArg_ParseTuple(args, "Of", &capsule, &scalar)) return NULL;

    Vector *v = (Vector*)PyCapsule_GetPointer(capsule, "Vector");
    if (!v) return NULL;

    if (scalar == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "Division by zero");
        return NULL;
    }

    Vector *result = scalar_div(v, scalar);

    return PyCapsule_New((void*)result, "Vector", NULL);
}

static PyObject* py_vec_mod(PyObject* self, PyObject* args){
    PyObject *capsule;
    // Only one argument: the Vector capsule
    if (!PyArg_ParseTuple(args, "O", &capsule)) return NULL;
    Vector *v = (Vector*)PyCapsule_GetPointer(capsule, "Vector");
    if (!v) return NULL;
    float result = vec_mod(v);  // call your C function
    // Return as Python float
    return PyFloat_FromDouble((double)result);
}

static PyObject* py_vec_dot(PyObject *self, PyObject *args){
    PyObject *capsule1, *capsule2;
    if (!PyArg_ParseTuple(args, "OO", &capsule1, &capsule2)) return NULL;
    Vector *v1 = (Vector*)PyCapsule_GetPointer(capsule1, "Vector");
    Vector *v2 = (Vector*)PyCapsule_GetPointer(capsule2, "Vector");

    if (!v1 || !v2) return NULL;

    if (v1->size != v2->size) {
        PyErr_SetString(PyExc_ValueError, "Vector sizes do not match");
        return NULL;
    }

    float result = dot(v1, v2);
    return PyFloat_FromDouble((double)result);
}

static PyObject* py_create_mat(PyObject* self, PyObject* args){
    PyObject* py_list;

    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_list)){
        return NULL;
    }
    int rows = (int)PyList_Size(py_list);
    if (rows <= 0) {
        PyErr_SetString(PyExc_ValueError, "Matrix must have at least one row");
        return NULL;
    }

    // Get number of columns from the first row
    PyObject* first_row = PyList_GetItem(py_list, 0);
    if (!PyList_Check(first_row)) {
        PyErr_SetString(PyExc_TypeError, "Matrix must be a list of lists");
        return NULL;
    }
    int cols = (int)PyList_Size(first_row);
    if (cols <= 0) {
        PyErr_SetString(PyExc_ValueError, "Matrix rows must have at least one element");
        return NULL;
    }

    // Create C matrix
    Matrix* m = create_mat(rows, cols);

    // Copy data from Python lists to C array
    for (int i = 0; i < rows; i++) {
        PyObject* row = PyList_GetItem(py_list, i);
        if (!PyList_Check(row) || PyList_Size(row) != cols) {
            free_mat(m);
            PyErr_SetString(PyExc_TypeError, "All rows must have same number of columns");
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            PyObject* item = PyList_GetItem(row, j);
            if (!PyFloat_Check(item) && !PyLong_Check(item)) {
                free_mat(m);
                PyErr_SetString(PyExc_TypeError, "Matrix elements must be numbers");
                return NULL;
            }
            m->data[i][j] = (float)PyFloat_AsDouble(item);
        }
    }

    // Wrap in PyCapsule to return to Python
    return PyCapsule_New((void*)m, "Matrix", NULL);
}

static PyObject* py_free_mat(PyObject* self, PyObject* args){
    PyObject* capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }

    Matrix* m = (Matrix*)PyCapsule_GetPointer(capsule, "Matrix");
    if (m == NULL) return NULL;

    free_mat(m);

    Py_RETURN_NONE;
}

static PyObject* py_print_mat(PyObject* self, PyObject* args){
    PyObject* capsule;

    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }

    Matrix* m = (Matrix*)PyCapsule_GetPointer(capsule, "Matrix");
    if (m == NULL) return NULL;

    print_mat(m);

    Py_RETURN_NONE;
}

static PyObject* py_mat_mul(PyObject* self, PyObject* args){
    PyObject *capsule1, *capsule2;
    if(!PyArg_ParseTuple(args,"OO",&capsule1,&capsule2)){
        return NULL;
    }
    Matrix* mat1 = (Matrix*)PyCapsule_GetPointer(capsule1,"Matrix");
    Matrix* mat2 = (Matrix*)PyCapsule_GetPointer(capsule2,"Matrix");

    if (!mat1 || !mat2) {
        PyErr_SetString(PyExc_ValueError, "Invalid matrix capsules");
        return NULL;
    }

    // Call your C matrix multiplication
    Matrix* result = mat_mul(mat1, mat2);
    if (!result) {
        PyErr_SetString(PyExc_ValueError, "Matrix dimensions do not match for multiplication");
        return NULL;
    }

    // Wrap the result in a PyCapsule and return
    return PyCapsule_New((void*)result, "Matrix", NULL);
}

static PyObject* py_mat_transpose(PyObject* self, PyObject* args){
    PyObject* capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    Matrix* m = (Matrix*)PyCapsule_GetPointer(capsule, "Matrix");
    if (m == NULL) return NULL;

    Matrix* res = mat_transpose(m);
    return PyCapsule_New((void*)res, "Matrix", NULL);
}

static PyMethodDef MiniMLMethods[] = {
    {"py_create_vec", py_create_vec, METH_VARARGS, "Create a vector"},
    {"py_print_vec", py_print_vec, METH_VARARGS, "Print a vector"},
    {"py_free_vec", py_free_vec, METH_VARARGS, "Free a vector"},
    {"py_vec_add", py_vec_add, METH_VARARGS, "Vector Addition"},
    {"py_vec_sub", py_vec_sub, METH_VARARGS, "Vector Subtraction"},
    {"py_scalar_mul", py_scalar_mul, METH_VARARGS, "Scalar Multiplication of vector"},
    {"py_scalar_div", py_scalar_div, METH_VARARGS, "Scalar Divison of vector"},
    {"py_vec_mod", py_vec_mod, METH_VARARGS, "Magnitude of vector"},
    {"py_vec_dot",py_vec_dot,METH_VARARGS,"Dot Product of vectors"},
    {"py_create_mat", py_create_mat, METH_VARARGS, "Create a matrix"},
    {"py_print_mat", py_print_mat, METH_VARARGS, "Print a matrix"},
    {"py_free_mat", py_free_mat, METH_VARARGS, "Free a matrix"},
    {"py_mat_mul", py_mat_mul, METH_VARARGS, "Matrix Multiplication"},
    {"py_mat_transpose",py_mat_transpose,METH_VARARGS,"Matrix transpose"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mini_ml_module = {
    PyModuleDef_HEAD_INIT,
    "mini_ml",
    "Mini ML library",
    -1,
    MiniMLMethods
};

PyMODINIT_FUNC PyInit_mini_ml(void) {
    return PyModule_Create(&mini_ml_module);
}
