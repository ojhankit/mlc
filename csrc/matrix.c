#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

Matrix* create_mat(int rows, int cols) {
    Matrix* m = (Matrix*) malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (float**) malloc(rows * sizeof(float*));
    for(int i=0; i<rows; i++)
        m->data[i] = (float*) malloc(cols * sizeof(float));
    return m;
}

void free_mat(Matrix* m) {
    if(m) {
        for(int i=0; i<m->rows; i++)
            free(m->data[i]);
        free(m->data);
        free(m);
    }
}

void print_mat(Matrix* m) {
    for(int i=0; i<m->rows; i++) {
        for(int j=0; j<m->cols; j++)
            printf("%f ", m->data[i][j]);
        printf("\n");
    }
}

Matrix* mat_mul(Matrix* mat1, Matrix* mat2){
    if(mat1->cols != mat2->rows){
        return NULL;
    }

    int rows = mat1->rows;
    int cols = mat2->cols;
    int common = mat1->cols;

    Matrix* res = create_mat(rows, cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            res->data[i][j]=0.0f;
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < common; k++) {
                res->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
            }
        }
    }
    return res;
}