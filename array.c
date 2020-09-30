#include <stdio.h>
#include <stdlib.h>
#include "array.h"

/*Mallocs a new performace structure and sets everything to the default values*/
struct Performance * newPerformance(){
    struct Performance * emptyPerformance = malloc(sizeof(struct Performance) * 1);
    emptyPerformance->reads = 0;
    emptyPerformance->writes = 0;
    emptyPerformance->mallocs = 0;
    emptyPerformance->frees = 0;
    return(emptyPerformance);
}
/*Mallocs a new array structure and sets everything to the default values*/
struct Array * newArray(struct Performance * performance, unsigned int width, unsigned int capacity){
    struct Array * emptyArray = malloc(sizeof(struct Array));

    if(emptyArray == NULL){
        fprintf(stderr,"Error: Not enough memory to malloc a array\n");
        exit(1);
    }

    emptyArray->data = malloc(sizeof(char) * (width * capacity));

    if(emptyArray->data == NULL){
        fprintf(stderr,"Error: Not enough memory to malloc array->data\n");
        exit(1);
    }

    emptyArray->width = width;
    emptyArray->nel = 0;
    emptyArray->capacity = capacity;
    performance->mallocs++;
    return(emptyArray);
}
/*Reads a item from the array into a destination pointer*/
void readItem(struct Performance * performance, struct Array * array, unsigned int index, void * dest){
    int numOfBytes = 0;
    int startLocation = (index * array->width); 
    int destIndex = 0;
    unsigned char * byteArray = array->data;
    unsigned char * destArray = dest;

    if(index >=array->nel){
        fprintf(stderr,"Error: Index is greater than or equal to the number of elements in array\n");
        exit(1);
    }
    
    for(numOfBytes = startLocation; numOfBytes < (startLocation+array->width);numOfBytes++ ){
        destArray[destIndex] = byteArray[numOfBytes];
        destIndex++;
    }
    dest = destArray;
    performance->reads++;
}
/*Writes a item from a src pointer to the array structure */
void writeItem(struct Performance * performance, struct Array * array, unsigned int index, void * src){

    int numOfBytes = 0;
    int startLocation = (index * array->width); 
    int srcIndex = 0;
    unsigned char * byteArray = array->data;
    unsigned char * srcArray = src;

    if(index > array->nel || index >= array->capacity){
        fprintf(stderr,"Error: Index is greater than array->nel or greater than or equal to array->capacity.\n");
        exit(1);
    }

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
/*Takes the array number of elements and subtracts one */
void contract(struct Performance * performance, struct Array * array){
    if(array->nel <= 0){
        fprintf(stderr,"Error: Can't contract a array with nothing in it.\n");
        exit(1);
    }
    array->nel--;
}
/* Frees the array structure*/
void freeArray(struct Performance * performance, struct Array * array){
    free(array->data);
    array->data = NULL;
    free(array);
    array = NULL;
    performance->frees++;
}
/*Add a item to the end of a array structure*/
void appendItem(struct Performance * performance, struct Array * array, void * src){
    writeItem(performance,array,array->nel,src);
}
/*Inserts a item into any part of the array structure*/
void insertItem(struct Performance * performance, struct Array * array, unsigned int index, void * src){
    int i = 0;
    void * itemToBeMoved = malloc(sizeof(char) * array->width);
    /*Index is cast to a int because if i is less than zero and its compared to a unsigned value of index i will become a massive unsigned value 
    which will cause the for loop to run until it runs out of room.*/
    for(i = array->nel-1; i >= (int)index; i = i-2){
        readItem(performance,array,i,itemToBeMoved);
        i++;
        writeItem(performance,array,i,itemToBeMoved);
    }

    free(itemToBeMoved);
    writeItem(performance,array,index,src);
    
}
/*Adds a item to the start of the array structure*/
void prependItem(struct Performance * performance, struct Array * array, void * src){
    insertItem(performance,array,0,src);
}
/*Delets a item from the array structure*/
void deleteItem(struct Performance * performance ,struct Array * array , unsigned int index){
    int i = 0;
    void * itemToBeMoved = malloc(sizeof(char) * array->width);

    for(i = (int)index+1; i<array->nel;i+=2){
        readItem(performance,array,i,itemToBeMoved);
        writeItem(performance,array,--i,itemToBeMoved);
    }

    free(itemToBeMoved);
    contract(performance,array);
}
/*Finds a item in the array structure using a sorted search*/
int findItem(struct Performance * performance, struct Array * array, int (*compar)(const void *, const void *), void * target){
    void * arrayByte = malloc(sizeof(char) * array->width);
    int i = 0;

    for(i = 0; i < array->nel; i++){
        readItem(performance,array,i,arrayByte);
        if(((*compar)(target,arrayByte)) == 0){
            free(arrayByte);
            return(i);
        }
    }
    free(arrayByte);
    return(-1);
   
}
/*Searches for a item in the array structure using a binary search*/
int searchItem(struct Performance * performance, struct Array * array, int (*compar)(const void *, const void *), void * target){
    void * arrayByte = malloc(sizeof(char) * array->width);
    int first = 0;
    int last = (int)array->nel-1;
    int middle = (first+last)/2;
    int result = 0;
    
    while(first <= last){
        readItem(performance,array,middle,arrayByte);
        result = (*compar)(target,arrayByte);
        if( result == 0){
            free(arrayByte);
            return(middle);
        }
        else if(result < 0){
            last = middle-1;
        }
        else{
            first = middle+1;
        }
        middle = (first+last)/2;
    }
    free(arrayByte);
    return(-1);
}


