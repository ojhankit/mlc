#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float* data;
    int size;
} Vector;

Vector* init_vec(int size);
void free_vec(Vector* v);
void print_vec(Vector* v);

Vector* vec_add(const Vector* v1, const Vector* v2);
Vector* vec_sub(const Vector* v1, const Vector* v2);
Vector* scalar_mul(Vector* v1, float scalar);
Vector* scalar_div(Vector* v1, float scalar);
float vec_mod(const Vector* v1);

#endif
