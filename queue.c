#include "queue.h"

Queue *createQueue(){
    Queue *queue = (Queue*)malloc(sizeof(Queue)); //Dynamically allocated space for a queue
    if(queue == NULL) {
        ERROR("Queue failed to malloc");
        exit(EXIT_FAILURE);
     }
    queue -> size = 0;
    queue -> front = 0; 
    queue -> end = ARRAY_SIZE - 1; 
    pthread_mutex_init(&queue -> lock, NULL);
    sem_init(&queue -> space_avil, 0, ARRAY_SIZE); 
    sem_init(&queue -> item_avil, 0, 0);
    return queue; 
}

//Enqueue
void enqueue(Queue *queue, char item[MAX_NAME_LENGTH]){
    sem_wait(&queue -> space_avil);
    pthread_mutex_lock (&queue -> lock);
        queue -> end = (queue -> end + 1) % ARRAY_SIZE; 
        char *node = item; 
        queue -> array[queue -> end] = node;
        queue -> size = queue -> size + 1;
    pthread_mutex_unlock (&queue -> lock);
    sem_post(&queue -> item_avil);
}

//Dequeue
char *dequeue(Queue *queue){
    sem_wait(&queue -> item_avil);
    pthread_mutex_lock(&queue -> lock);
        char *node = queue ->array[queue -> front];
        queue -> front = (queue -> front + 1) % ARRAY_SIZE; 
        queue -> size = queue -> size - 1;
    pthread_mutex_unlock(&queue -> lock);
    sem_post(&queue -> space_avil);
    return node;
}

int queueSize(Queue *queue){
    pthread_mutex_lock(&queue -> lock); 
        int qSize = queue -> size; 
    pthread_mutex_unlock(&queue -> lock); 
    return qSize; 
}

//Destroy allocated Memory 
void destroyQueue(Queue *queue){
    sem_destroy(&queue -> space_avil);
    sem_destroy(&queue -> item_avil);
    pthread_mutex_destroy(&queue -> lock);
    free(queue); 
}
