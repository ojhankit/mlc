#include<stdio.h>
#include<stdlib.h>
#include<math.h>
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

// operations
Vector* vec_add(const Vector*v1, const Vector*v2){
    if(v1->size != v2->size){
        printf("size not same\n");
        return NULL;
    }
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->size = v1->size;
    v->data = (float*) malloc(sizeof(float) * v->size);
    for(int i=0;i<v->size;i++){
        v->data[i] = v1->data[i] + v2->data[i];
    }
    return v;
}

Vector* vec_sub(const Vector* v1, const Vector* v2){
    if(v1->size != v2->size){
        printf("size not same\n");
        return NULL;
    }
    Vector* v = (Vector*)malloc(sizeof(Vector));
    v->size = v1->size;
    v->data = (float*) malloc(sizeof(float) * v->size);
    for(int i=0;i<v->size;i++){
        v->data[i] = v1->data[i] - v2->data[i];
    }
    return v;
}

Vector* scalar_mul(Vector* v1, float scalar){
    for(int i=0;i<v1->size;i++){
        v1->data[i] *= scalar;
    }
    return v1;
}

Vector* scalar_div(Vector* v1, float scalar){
    for(int i=0;i<v1->size;i++){
        v1->data[i] /= scalar;
    }
    return v1;
}

float vec_mod(const Vector* v){
    float sum = 0;
    for(int i=0;i<v->size;i++){
        sum += pow(v->data[i],2);
    }
    return sqrt(sum);
}