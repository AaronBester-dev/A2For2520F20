#include <stdio.h>
#include <stdlib.h>
#include "array.h"

struct Performance * newPerformance(){
    struct Performance * emptyPerformance = malloc(sizeof(struct Performance) * 1);
    emptyPerformance->reads = 0;
    emptyPerformance->writes = 0;
    emptyPerformance->mallocs = 0;
    emptyPerformance->frees = 0;
    return(emptyPerformance);
}

struct Array * newArray(struct Performance * performance, unsigned int width, unsigned int capacity){
    struct Array * emptyArray = malloc(sizeof(struct Array));
    if(emptyArray == NULL){
        fprintf(stderr,"Error Not enough memory to malloc a array\n");
        exit(1);
    }
    emptyArray->data = malloc(sizeof(char) * (width * capacity));
    if(emptyArray->data == NULL){
        fprintf(stderr,"Error Not enough memory to malloc data in an array\n");
        exit(1);
    }
    emptyArray->width = width;
    emptyArray->nel = 0;
    emptyArray->capacity = capacity;
    performance->mallocs++;
    return(emptyArray);
}

void readItem(struct Performance * performance, struct Array * array, unsigned int index, void * dest){
    
    if(index >=array->nel){
        fprintf(stderr,"Error Index is greater than or equal to the number of elements in array\n");
        exit(1);
    }

    int numOfBytes = 0;
    int startLocation = (index * array->width); 
    int destIndex = 0;
    unsigned char * byteArray = array->data;
    unsigned char * destArray = dest;

    for(numOfBytes = startLocation; numOfBytes < (startLocation+array->width);numOfBytes++ ){
        destArray[destIndex] = byteArray[numOfBytes];
        destIndex++;
    }
    dest = destArray;
    performance->reads++;
}

void writeItem(struct Performance * performance, struct Array * array, unsigned int index, void * src){
    if(index > array->nel || index >= array->capacity){
        fprintf(stderr,"Error writing item.\n");
        exit(1);
    }

    int numOfBytes = 0;
    int startLocation = (index * array->width); 
    int srcIndex = 0;
    unsigned char * byteArray = array->data;
    unsigned char * srcArray = src;

    for(numOfBytes = startLocation; numOfBytes < (startLocation+array->width); numOfBytes++){
        byteArray[numOfBytes] = srcArray[srcIndex];
        srcIndex++;
    }
    array->data = byteArray;
    if(index == array->nel){
        array->nel++;
    }
    performance->writes++;
}

void contract(struct Performance * performance, struct Array * array){
    if(array->nel <= 0){
        fprintf(stderr,"Error: Can't contract a array with nothing in it.\n");
        exit(1);
    }
    array->nel--;
}

void freeArray(struct Performance * performance, struct Array * array){
    free(array->data);
    free(array);
    array = NULL;
    performance->frees++;
}

void appendItem(struct Performance * performance, struct Array * array, void * src){
    writeItem(performance,array,array->nel,src);
}

void insertItem(struct Performance * performance, struct Array * array, unsigned int index, void * src){
    int i = 0;
    void * itemToBeMoved = NULL;
    for(i = array->nel-1; i>=index;i-=2){
        readItem(performance,array,i,itemToBeMoved);
        writeItem(performance,array,i+1,itemToBeMoved);
    }
    writeItem(performance,array,index,src);
}

void prependItem(struct Performance * performance, struct Array * array, void * src){
    insertItem(performance,array,0,src);
}

void deleteItem(struct Performance * performance ,struct Array * array , unsigned int index){
     int i = 0;
    void * itemToBeMoved = NULL;
    for(i = array->nel-1; i>=index+1;i-=2){
        readItem(performance,array,i,itemToBeMoved);
        writeItem(performance,array,i+1,itemToBeMoved);
    }
    contract(performance,array);
}



