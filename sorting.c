/**
 Nicholas Latham
 project 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	for(i = 0; i < SIZE; i++){
		printf("%d ", list[i]);
	}
	printf("\n");

	/* establish the first sorting thread */
	parameters *data = (parameters *) malloc (SIZE/2*sizeof(parameters));
	data->from_index = 0;
	data->to_index = (SIZE/2)-1;
	pthread_create(&workers[0], 0, sorter, data);
	

	/* establish the second sorting thread */	
    data = (parameters *) malloc (SIZE/2*sizeof(parameters));
    data->from_index = (SIZE/2);
    data->to_index = SIZE - 1;
    pthread_create(&workers[1], 0, sorter, data);
  
	
	
	/* now wait for the 2 sorting threads to finish */
	for (i = 0; i < NUMBER_OF_THREADS - 1; i++){
        pthread_join(workers[i], NULL);
	}

	/* establish the merge thread */
	data = (parameters *) malloc (SIZE*sizeof(parameters));
    data->from_index = 0;
    data->to_index = SIZE - 1;
    pthread_create(&workers[2], 0, merger, data);
	
	/* wait for the merge thread to finish */
	pthread_join(workers[2], NULL);


	/* output the sorted array */
	for(i = 0; i < SIZE; i++){
		printf("%d ", result[i]);
	}
	printf("\n");
	
    return 0;
}

/**
 * Sorting thread.
 */

void *sorter(void *params)
{
    parameters* p = (parameters *)params;
 
    int begin = p->from_index;
    int end = p->to_index+1;

    int temp=0;
    for(int i=begin; i< end; i++){
        for(int j=begin; j< end-1; j++){
             if(list[j] > list[j+1]){
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
             }
        }
    }
 
    pthread_exit(NULL);
}
 
/*
merging thread
 */
void *merger(void *params)
{
    parameters* p = (parameters *)params;
    int begin = p->from_index;
    int end = p->to_index+1;
    int half = end/2;
    int j, i, k;
    for(i = 0, j = half, k = 0; i < half && j < end; k++){
        if(list[i] < list[j]){
            result[k] = list[i];
            i++;
        }else{
            result[k] = list[j];
            j++;
        }
    }
    while(j < end){
        result[k] = list[j];
        j++; k++;
    }
    while(i < half){
        result[k] = list[i];
        i++; k++;
    }
 
    pthread_exit(NULL);
}
