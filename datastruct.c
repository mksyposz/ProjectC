#include"datastruct.h"

void vector_init(struct vector *v)
{ 
    v->data = NULL; 
    v->size = 0; 
    v->count = 0;
} 

void vector_add(struct vector *v,void *p) 
{ 
    if(v->size == 0) 
    { 
        v->size = 10; 
        v->data = malloc(sizeof(void*)*v->size); 
    } 
    if(v->size == v->count) 
    { 
        v->size *= 2; 
        v->data = realloc(v->data, sizeof(void*) * v->size); 
    } 

    v->data[v->count] = p; 
    v->count++;
} 
