#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    float** data;
    int rows;
    int cols;
} Matrix;

Matrix* create_mat(int rows, int cols);
void free_mat(Matrix* m);
void print_mat(Matrix* m);

#endif