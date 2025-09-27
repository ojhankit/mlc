#include<stdio.h>
#include<stdlib.h>
#include "vector.h"

Vector* init_vec(int size){
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->size = size;
    v->data = (float*) malloc(sizeof(float) * size);
    return v;
}

void free_vec(Vector* v){
    if(v){
        free(v->data);
        free(v);
    }
}

void print_vec(Vector* v){
    for(int i=0;i<v->size;i++){
        printf("%f ",v->data[i]);
    }
    printf("\n");
}