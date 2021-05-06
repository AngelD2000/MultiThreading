#ifndef QUEUE_H
#define QUEUE_H

#include "multi-lookup.h"


struct Queue{
    int front, end, size;
    char *array[ARRAY_SIZE];
    sem_t space_avil; //Semaphore that counts how many spaces are left in common buffer
    sem_t item_avil; //Semaphore that counts how many items are in common buffer
    pthread_mutex_t lock;
}; 
typedef struct Queue Queue; 

//Function to create a queue
Queue *createQueue();

//Enqueue
void enqueue(Queue *queue, char item[MAX_NAME_LENGTH]);

//Dequeue
char *dequeue(Queue *queue);

int queueSize(Queue *queue);

//Destroy allocatd memory
void destroyQueue(Queue *queue); 

#endif
