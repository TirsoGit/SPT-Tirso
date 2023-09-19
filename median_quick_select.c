#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define FILTER_WINDOW_SIZE 3

/* Median filter data structure */
typedef struct {
    int window;   // Size of the window
    int *data;    // Working buffer
    int *samples; // Shift left register of original samples (could be removed in a real app.)
} filter_t;

/* Public Functions */
void filter_init(filter_t *f, int n, int init_val);
int filter_add(filter_t *f, int value);
void filter_destroy(filter_t* f);

/* Private functions */
void filter_shift(filter_t *f, int value); // Not needed in a real app.
void filter_swap(int* a, int* b);
int filter_partition(filter_t *f, int l, int r);
int filter_rand_partition(filter_t *f, int l, int r);
void filter_util(filter_t *f, int l, int r, int* a, int* b);
int filter_median(filter_t *f);

// Initializes filter filling buffer with 'init_val' values.
void filter_init(filter_t *f, int n, int init_val) {
    if(f==NULL) return;
    f->data=(int*)malloc(n*sizeof(int)); // allocs working buffer
    if(f->data==NULL) return;
    f->samples=(int*)malloc(n*sizeof(int)); // allocs shift register
    if(f->samples==NULL){ free(f->data); return; }
    f->window=n; // inits windows size
    for(int i=0;i<n;i++)
        f->data[i]=f->samples[i]=init_val; // fills buffers
}

// Executes the median filter on a new sample 'value'.
int filter_add(filter_t *f, int value){
    (void)filter_shift(f,value); // Adds the new value and remove the oldest.
    memcpy(f->data,f->samples,f->window*sizeof(int)); // 
    return filter_median(f);
}

// Removes allocated memory.
void filter_destroy(filter_t* f){
    if(f==NULL) return;
    free(f->data);
    free(f->samples);
}

// Adds a new sample 'value' in the shift left register removing the oldest sample. 
void filter_shift(filter_t *f, int value){
    memmove((void*)&f->samples[0],(void*)&f->samples[1],(f->window-1)*sizeof(int));
    f->samples[f->window-1]=value; // Adds the newest to head
}

// Swaps two elements referenced by pointers.
void filter_swap(int* a, int* b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

// Arranges (not sort it) f->data[l..r] placing all values less than pivot to the left of it 
// and all values greater than pivot to the right of it. Returns the correct position 
// (index) of the pivot.
int filter_partition(filter_t *f, int l, int r)
{
    int i, j, vpivot=f->data[r];
    /*
       Every value [j] on f->data[l..r-1] less than f->data[r] (value of pivot)
       is swapped to the left (indexed by [i]). The process terminates with: 
        - all values less than pivot before [i]
        - all values greater than pivot after [i]
        - the value at [i] is the pivot
    */
    for(i=j=l; j < r; j++){
        if(f->data[j] < vpivot){
            filter_swap(&f->data[i],&f->data[j]); // Swaps [j] values less than pivot to the left [i]
            i++; // Points to the next...
        } // else: dont increments i, when [j] value is greater than pivot.
    }
    // Moves pivot [r] to their correponding index position [i].
    filter_swap(&f->data[i],&f->data[r]);
    return i;
}

// Picks a random pivot element between l and r and partitions f->data[l..r]
// around the randomly picked element using filter_partition()
int filter_rand_partition(filter_t *f, int l, int r)
{
    int n = r - l + 1;  // Calculates the number of elements n in f->data[] between l and r
    int pivot = rand() % n; // Takes a random number between 0..n-1 (the pivot)
    /* 
       Swaps the value on pivot (left+pivot) with the value on last position following
       the standard partition process of QuickSort(). It considers the last element 
       as pivot and moves all smaller element to left of it and greater elements 
       to right
    */
    filter_swap(&f->data[l+pivot],&f->data[r]); 
    // Partitions f->data[l..r] and returns the pivot index.
    return filter_partition(f,l,r);
}

// Utility function to find the median.
void filter_util(filter_t *f, int l, int r, int* a, int* b)
{
    if(l <= r){
         // Finds a random partition index and arranges all values around it.
        int partitionIndex = filter_rand_partition(f,l,r);
        /* 
          If partition index == k, then we found the median of odd number 
          element in f->data[].
        */
        if(partitionIndex == f->window/2){
            *b = f->data[partitionIndex];
            if (*a != -1)
                return;
        }
        /* 
          If index == k - 1, then we get a & b as middle element of f->data[].
        */
        else if(partitionIndex == f->window/2-1){
            *a = f->data[partitionIndex];
            if(*b != -1)
                return;
        }
 
        /* 
           If partitionIndex >= k then finds index in first half of the f->data[].
           If partitionIndex < k then finds index in second half of the f->data[].
         */
        if(partitionIndex >= f->window/2)
            return filter_util(f, l, partitionIndex - 1, a, b);
        else
            return filter_util(f, partitionIndex + 1, r, a, b);
    }
     return;
}

// Function to find Median
int filter_median(filter_t *f)
{
    int ans, a = -1, b = -1;
 
    // Gets values from center of the window in a and b.
    filter_util(f,0,f->window-1,&a,&b);

    /*
       If window is odd gets b from window/2
       If window is even gets a from (windows/2)-1 and b from windows/2.
    */
    return ((f->window % 2)==1) ? b : (a+b)/2;
}

// Auxiliary function.
void displayBuffers(filter_t *f){
    for(int i=0;i<f->window;i++)
        printf(" [%d] data=%d, samples=%d ",i,f->data[i],f->samples[i]);
}

// Auxiliary function.
void displayArray(int *s, int n){
    for(int i=0;i<n;i++)
        printf("[%d],",s[i]);
}

int main() {
    /* Test case */
    int samples[] = { 3,9,4,52,3,8,6,2,2,9,9 };
    //int samples[] = { 255,10,9,10,255,10,8,10,10,255,10,9,10,255,10,8,10,10 };
    //int samples[] = { 124,126,127,120,150,125,115,119,123,124,126,127,120,150,125,115,119,123 };
    int nSamples=sizeof(samples)/sizeof(int);
    
    filter_t oneMedianFilter;
    filter_init(&oneMedianFilter,FILTER_WINDOW_SIZE,samples[0]);
    printf("\nSamples: ");
    displayArray(samples,nSamples);
    printf("\nMedians: ");
    for(int i=0;i<nSamples;i++){
        int tmp=filter_add(&oneMedianFilter,samples[i]);
        printf("[%d],",tmp);
    }

    return 0;
}
