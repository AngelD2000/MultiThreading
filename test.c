#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

int main(int argc, char **argv){
    char *input; 
    int ret; 
    printf("Enter: ");
    fgets(input, 20, stdin);
    char *hello = "Hello"; 
    ret = strcmp(input, hello); 
    if(ret == 0){
        printf("OK\n");
    }
    else{
        printf("NO\n");
    }
    return 0;
}