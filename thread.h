#ifndef THREAD_H
#define THREAD_H

#include "queue.h"
#include "multi-lookup.h"
#include "util.h"

#define OutputLog(S, lock){pthread_mutex_lock(lock); S; pthread_mutex_unlock(lock);}

struct Thread_arg{
    Queue *queue; 
    char **argvDup;
    int argvpos;
    int numAssigned; 
    int numFileServiced; 
    int totalFiles; 
    FILE *service_file; 
    FILE *resolve_file;    
    pthread_mutex_t argv_lock; 
    pthread_mutex_t readWrite_lock;
    pthread_mutex_t stderr_lock;
    pthread_mutex_t servPrint; 
    pthread_mutex_t resPrint; 
    pthread_mutex_t serviceCount_lock; 
};

typedef struct Thread_arg Thread_arg;
Thread_arg *createThreadArgs(Queue *queue, char **argvDup, int argvPosition, int totalFiles, FILE *serFile, FILE *resFile);

void inputToBuffer(Thread_arg *argParam, FILE *fp);
void *requester(void *args);

void *resolver(void *args);

void destroyThreadArgs(Thread_arg *arg);

#endif