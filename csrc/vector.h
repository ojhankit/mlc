#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float* data;
    int size;
} Vector;

Vector* init_vec(int size);
void free_vec(Vector* v);
void print_vec(Vector* v);

#endif
