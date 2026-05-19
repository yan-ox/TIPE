#include "..//img_manage.h"
#include "..//basics.h"

#include <stdlib.h>
#include <stdio.h>

void print_arr(float* arr, int n);  //done but will be deleted 

void select_ksort(float* arr, int n, int k);  //done
int* KNN(int i, img* image);    //to do
void full_apply(img* image);    //to do

void select_ksort(float* arr, int n, int k){
    /*Will sort the first k index of tab
    Needed: k <= n
    */
    for(int i = 0; i < k; i ++){
        int min_id = minf(arr + i, n - i) + i;
        float mini = arr[min_id];
        arr[min_id] = arr[i];
        arr[i] = mini;
    }
}

int* KNN(int i, img* image){
    float** 
    //Calculate all distances
    //
}

void print_arr(float* arr, int n){
    for (int i = 0; i < n; i ++){
        printf("%f, ", arr[i]);
    }
}

void main(){
    /*
    float arr1[12] = {3.2, 1.5, 9.8, 4.4, 2.1, 7.7, 6.3, 0.9, 5.5, 8.6, 2.8, 1.0};

    float arr2[13] = {10.5, 3.3, 7.1, 2.2, 9.9, 4.8, 6.6, 1.4, 8.0, 5.7, 3.9, 0.6, 2.5};

    float arr3[11] = {0.2, 5.5, 3.3, 9.1, 7.4, 6.8, 2.6, 8.9, 1.7, 4.0, 3.1};

    float arr4[14] = {12.1, 3.8, 6.4, 2.9, 11.0, 5.6, 7.3, 0.5, 9.2, 4.7, 8.8, 1.3, 10.6, 2.0};

    float arr5[15] = {1.1, 4.4, 2.2, 9.9, 3.3, 7.7, 6.6, 5.5, 8.8, 0.0, 2.9, 4.1, 6.2, 3.7, 1.8};
    select_ksort(arr1, 12, 5);
    select_ksort(arr2, 13, 5);
    select_ksort(arr3, 11, 5);
    select_ksort(arr4, 14, 5);
    select_ksort(arr5, 15, 5);
    print_arr(arr1, 12);
    printf("\n");
    print_arr(arr2, 13);
    printf("\n");
    print_arr(arr3, 11);
    printf("\n");
    print_arr(arr4, 14);
    printf("\n");
    print_arr(arr5, 15);
    printf("\n");
    */
}