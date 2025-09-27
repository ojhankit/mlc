#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <immintrin.h>   // AVX, SSE, etc.
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

float dot(const Vector* v1, const Vector* v2){
    float sum = 0.0f;
    if(v1->size != v2->size){
        fprintf(stderr,"size not same\n");
        return 0.0f;
    }
    /*for(int i=0;i<v1->size;i++){
        sum += (v1->data[i] * v2->data[i]);
    }*/
    int size = v1->size;
    float* d1 = v1->data;
    float* d2 = v2->data;

    int i = 0;
    int unroll_factor = 4;
    int limit = size - (size % unroll_factor);

    // loop unrolling 4 elements at a time
    for (; i < limit; i+=unroll_factor){
        sum += d1[i] * d2[i];
        sum += d1[i+1] * d2[i+1];
        sum += d1[i+2] * d2[i+2];
        sum += d1[i+3] * d2[i+3];
    }

    for(;i < size; i++){
        sum += d1[i] * d2[i];
    }
    return sum;
}