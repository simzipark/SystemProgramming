/*
2017 System Programming
Assignment #3
Fork Ver.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SIZE 1024

void quick_sort(int data[], int start, int quota);

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void quick_sort(int data[], int start, int end){
    int mid = (start+end)/2;
    int pivot = data[mid];

    if(start >= end) return;

    swap(&data[start], &data[mid]);

    int p = start+1, q = end;

    while(1){
        while(data[p] <= pivot) p++;
        while(data[q]>pivot) q--;

        if(p>q) break;

        swap(&data[p], &data[q]);
    }

    swap(&data[start], &data[q]);

    quick_sort(data, start, q-1);
    quick_sort(data, q+1, end);
}

