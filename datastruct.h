#ifndef DATASTRUCT_H_INCLUDED
#define DATASTRUCT_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>

struct pair_ii
{ 
    int first; 
    int second;
};

struct vector
{ 
    struct pair_ii **data;
    int count; 
    int size;
}; 

void vector_init(struct vector*); 
void vector_add(struct vector*,struct pair_ii*);
#endif
