#include "thread.h"

//Should be simple branch

Thread_arg *createThreadArgs(Queue *queue, char **argvDup, int pos, int totalFiles, FILE *serviceFile, FILE * resolveFile){
    Thread_arg *arg = (Thread_arg *)malloc(sizeof(Thread_arg)); 
    if(arg == NULL) {ERROR("Thread_arg failed to malloc"); exit(EXIT_FAILURE); }
    arg -> queue = queue; 
    arg -> argvDup = argvDup; 
    arg -> argvpos = pos;
    arg -> numAssigned = 0; 
    arg -> totalFiles = totalFiles; 
    arg -> service_file = serviceFile; 
    arg -> resolve_file = resolveFile; 
    arg -> numFileServiced = 0; 
    pthread_mutex_init(&arg -> argv_lock, NULL); 
    pthread_mutex_init(&arg -> readWrite_lock, NULL); 
    pthread_mutex_init(&arg -> output_lock, NULL); 
    pthread_mutex_init(&arg -> serviceCount_lock, NULL);
    return arg; 
}

void inputToBuffer(Thread_arg *arg, FILE *fp){
    Queue *queue = arg -> queue; 
    FILE *serviceFile = arg -> service_file;
    char line[MAX_NAME_LENGTH]; 
    char checkSize[MAX_NAME_LENGTH * 2]; 
    while(fgets(checkSize, MAX_NAME_LENGTH * 2 , fp) != NULL){
        if(strlen(checkSize) + 1 < MAX_NAME_LENGTH){
            strncpy(line, checkSize, MAX_NAME_LENGTH);
            if(line[strlen(line)-1] == '\n'){
                line[strlen(line)-1] = '\0';
            }
            char *dest = (char *) malloc(MAX_NAME_LENGTH);
            if(dest == NULL){
                OutputLog(ERROR("Dest failed to malloc"),&arg -> output_lock);
                exit(EXIT_FAILURE);
            }
            memcpy(dest, line, MAX_NAME_LENGTH);
            enqueue(queue,dest);
            OutputLog(fprintf(serviceFile, "%s\n",dest),&arg -> readWrite_lock);
        }
        else{
            OutputLog(ERROR("Length of hostname is out of bounds. Moving on to the next hostname"),&arg -> output_lock);
        }
    }
}


void *requester(void *thread_args){
    Thread_arg *arg = (Thread_arg *)thread_args; 
    int numFilesServiced = 0;
    int count = 0;
    int i = 0; 
    FILE *fp; 
    while(1){
        pthread_mutex_lock(&arg -> argv_lock); 
            arg -> numAssigned++; 
            count = arg -> numAssigned; 
            i = arg -> argvpos; 
            arg -> argvpos++;
        pthread_mutex_unlock(&arg -> argv_lock);
        if(count > arg -> totalFiles) break; 
        fp = fopen(arg -> argvDup[i], "r"); 
        if(fp == NULL){
            OutputLog(fprintf(stderr,"invalid file <%s>\n",arg ->argvDup[i]),&arg -> output_lock);
        }
        else{
            inputToBuffer(arg, fp);
        }
        numFilesServiced += 1;
        pthread_mutex_lock(&arg -> serviceCount_lock); 
            arg -> numFileServiced++; 
        pthread_mutex_unlock(&arg -> serviceCount_lock);
        if(fp != NULL){
            fclose(fp);
        }
    } 
    OutputLog(fprintf(stdout,"thread <%lu> serviced %d files\n", pthread_self(), numFilesServiced),&arg -> output_lock);
    return 0;
}



void *resolver(void *thread_args){
    Thread_arg *arg = (Thread_arg *)thread_args; 
    Queue *queue = arg -> queue;
    FILE *resFile = arg -> resolve_file;
    int numResolved = 0;
    int qSize = 0;      
    int countServFile = 0;
    char addressNum[MAX_IP_LENGTH];
    while(1){
        pthread_mutex_lock(&arg -> serviceCount_lock); 
            countServFile = arg -> numFileServiced; 
            qSize = queueSize(queue);
        pthread_mutex_unlock(&arg -> serviceCount_lock);

        if((countServFile == arg ->totalFiles) && (qSize == 0) ) break; 
        char *name = dequeue(queue); 
        int flag = dnslookup(name, addressNum, MAX_IP_LENGTH);
        if(flag == 0){
            OutputLog(fprintf(resFile, "%s, %s\n", name, addressNum), &arg -> readWrite_lock);
            numResolved += 1; 
        }
        else{
            OutputLog( fprintf(resFile, "%s, NOT_RESOLVED\n", name), &arg -> readWrite_lock)
        }
        free(name);        
    }
    OutputLog(fprintf(stdout,"thread <%lu> resolved %d hostnames\n", pthread_self(), numResolved),&arg -> output_lock);
    return 0;
}


void destroyThreadArgs(Thread_arg *arg){
    pthread_mutex_destroy(&arg -> argv_lock); 
    pthread_mutex_destroy(&arg -> readWrite_lock); 
    pthread_mutex_destroy(&arg -> output_lock); 
    pthread_mutex_destroy(&arg -> serviceCount_lock);
    free(arg);
}