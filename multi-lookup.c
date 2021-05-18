#include "multi-lookup.h"
#include "queue.h"
#include "thread.h"

#include <sys/time.h> 
#include <pthread.h>

int main(int argc, char **argv){
    /* ERROR HANDELING */
    if(argc < MIN_ARG) {
        fprintf(stdout, "Not enough arguments\n"); 
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    if(argc > MAX_ARG) {ERROR("Too many arguments"); exit(EXIT_FAILURE);}
    if(argc - 5 > MAX_INPUT_FILES) {ERROR("Too many input files"); exit(EXIT_FAILURE);}
    if(atoi(argv[1]) == 0) {ERROR("Number of requester threads must be a number greater than 0"); exit(EXIT_FAILURE);}
    if(atoi(argv[2]) == 0) {ERROR("Number of resolver threads must be a number greater than 0"); exit(EXIT_FAILURE);}
    if(atoi(argv[1]) > MAX_REQUESTER_THREADS || atoi(argv[2]) > MAX_RESOLVER_THREADS) {
        ERROR("Max requester and resolver threads are 10");
        exit(EXIT_FAILURE); 
    }
    if(atoi(argv[3]) != 0) {ERROR("Requester log file name cannot be numbers."); exit(EXIT_FAILURE);}
    if(atoi(argv[4]) != 0) {ERROR("Resolver log file name cannot be numbers."); exit(EXIT_FAILURE);}
    FILE *service_file = fopen("serviced.txt","w"); 
    if(service_file == NULL){
        ERROR("Failed to open requester log file"); 
        exit(EXIT_FAILURE);
    }
    FILE *resolve_file = fopen("resolved.txt","w");
    if(resolve_file == NULL){
        ERROR("Failed to open resolver log file"); 
        exit(EXIT_FAILURE);
    }

    /* START PROGRAM */

    struct timeval start, end; 
    gettimeofday(&start,NULL);

    int num_req_thread = atoi(argv[1]); 
    int num_res_thread = atoi(argv[2]);

    pthread_t reqPool[num_req_thread];
    pthread_t resPool[num_res_thread];

    Queue *queue = createQueue(); 
    Queue *fileQueue = createQueue(); 

    Thread_arg *arg = createThreadArgs(queue, fileQueue, argc - MIN_ARG, service_file, resolve_file); 

    for(int i = 0; i < num_req_thread; i++){
        if(pthread_create(&reqPool[i], NULL, requester,(void *) arg)!=0) {
            ERROR("Cannot create requester threads");
        }
    }
    
    for(int j = 0; j < num_res_thread; j++){
        if(pthread_create(&resPool[j], NULL, resolver,(void *) arg)!=0) {
            ERROR("Cannot create resolver threads");
        }
    }

    for(int k = MIN_ARG; k < argc;k++){
        enqueue(fileQueue, argv[k]);
    }

    for(int a = 0; a < num_req_thread; a++ ){
        pthread_join(reqPool[a], NULL); 
    }

    for(int b = 0; b < num_res_thread; b++){
        pthread_join(resPool[b], NULL); 
    }    

    fclose(service_file); 
    fclose(resolve_file);

    destroyQueue(queue); 
    destroyQueue(fileQueue);
    destroyThreadArgs(arg);

    gettimeofday(&end, NULL);
    double time_sec = (end.tv_sec + (end.tv_usec/1000000.0)) - (start.tv_sec + ( start.tv_usec/1000000.0)); 
    fprintf(stdout,"%s: total time is %lf seconds\n", argv[0], time_sec);
    fflush(stdout); 
}
