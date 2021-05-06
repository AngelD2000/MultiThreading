#ifndef MULTILOOKUP_H
#define MULTILOOKUP_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ARRAY_SIZE 10 
#define MAX_NAME_LENGTH 255
#define MAX_INPUT_FILES 100
#define MAX_REQUESTER_THREADS 10
#define MAX_RESOLVER_THREADS 10
#define MAX_IP_LENGTH INET6_ADDRSTRLEN 

#define MIN_ARG 5
#define MAX_ARG 105

#define ERROR(S){fprintf(stderr, "Error: %s\n",S);}



#endif