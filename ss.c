#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXSZ 1000
typedef struct kv {  
    int value;
} node;

node *store[MAXSZ];

int main() {
    char buffer[50] = "put 1 2";
    char * token = strtok(buffer, " ");
    char * command = token;
    printf( " %s\n", command ); 
    token = strtok(NULL, " ");
    char * key = token;
    printf( " %s\n", key );
    token = strtok(NULL, " ");
    char * value = token;
    printf( " %s\n", value );
    node *n;
    n = (node *)malloc(sizeof(node));
    int k = atoi(key);
    n->value = atoi(value);
    if (store[k] == NULL) {
        store[k] = n;
        printf("store[%d]->value: %d\n", k,n->value);
        // bzero(buffer,MAX_INPUT_SIZE);
    }
    else {
        printf("key existed");
        // bzero(buffer,MAX_INPUT_SIZE);
    }

    char buffe[50] = "get 1";
    char * toke = strtok(buffe, " ");
    char * comman = toke;
    printf( " %s\n", comman ); 
    toke = strtok(NULL, " ");
    char * ke = toke;
    printf( " %s\n", ke );
    node *x;
    int ky = atoi(ke);
    x = store[ky];
    
    printf("get store[%d]->value: %d\n", ky,x->value);
        // bzero(buffer,MAX_INPUT_SIZE);
   
   return 0;
}