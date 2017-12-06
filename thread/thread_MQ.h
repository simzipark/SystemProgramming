/*
2017 System Programming
Assignment #3
Thread Ver.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define SIZE 1024
pthread_mutex_t mutex;

struct Parameters
{
    int *input;
    int num_of_inputs;
    int num_of_threads;
};

void merge_sort(int *input, int num_of_inputs, int num_of_threads);
void merge(int *input, int *mid, int *end);
void *merge_thread(void * pp);

void merge_sort(int *input, int num_of_inputs, int num_of_threads){
    if(num_of_inputs < 2){
        return;
    }
    /* Divide */
    if(num_of_threads <= 0 || num_of_inputs < 4){
        /* merge_sort(first thing) */
        merge_sort(input, num_of_inputs/2, 0);
        /* merge_sort(second & thirth thing) */
        merge_sort(input+num_of_inputs/2, num_of_inputs-num_of_inputs/2, 0);
    }
    else{
        struct Parameters prms = { input, num_of_inputs/2, num_of_threads/2 };

        /* create & operate threads */
        pthread_t thread;
        pthread_create(&thread, NULL, merge_thread, &prms);

        /* recurse top >> end */
        merge_sort(input+num_of_inputs/2, num_of_inputs-num_of_inputs/2, num_of_threads/2);

        /* join threads */
        pthread_join(thread, NULL);
    }

    /* Conquer */
    pthread_mutex_lock(&mutex);
    merge(input, input+num_of_inputs/2, input+num_of_inputs);
    pthread_mutex_unlock(&mutex);
}

void *merge_thread(void * pp){
    struct Parameters *prms = pp;
    merge_sort(prms->input, prms->num_of_inputs, prms->num_of_threads);
    return pp;
}

void merge(int *input, int *mid, int *end){
    int *temp = malloc((end-input)*sizeof(temp));  //for save sorted array
    int *left = input;  //LeftIndex
    int *right = mid;   //RightIndex
    int *dst = temp;    //DestIndex
    
    /* compare left side & right side and then sort & merge */
    while(left != mid && right != end)
        *dst++ = (*left < *right) ? *left++ : *right++;

    /* fill rest part of the input */
    while(left != mid)
        *dst++ = *left++;

    /* copy sorted data into input array */
    memcpy(input, temp, (right-input) * sizeof * temp);

    /* free malloc */
    free(temp);
}
