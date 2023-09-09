#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define FILTER_WINDOW_SIZE 3

/* Median filter data structure */
typedef struct {
    int window;     // size of the window
    int *data;      // working buffer
    int *samples;   // shift left register of samples (could be removed in a real app.)
} filter_t;

/* Public Functions */
void filter_init(filter_t *f, int n, int init_val);
int filter_add(filter_t *f, int value);
void filter_destroy(filter_t* f);

/* Private functions */
void filter_shift(filter_t *f, int value); // not needed in a real app.
int filter_cmp(const void *a, const void *b);

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

int filter_cmp(const void *a, const void *b){ 
    return *(const int *)a - *(const int *)b;
}

// Executes the median filter on a new sample 'value'.
int filter_add(filter_t *f, int value){
    (void)filter_shift(f,value);
    memcpy(f->data,f->samples,f->window*sizeof(int));
    qsort(f->data,f->window,sizeof(int),filter_cmp); // Complexity O(nlog n)
    int tmp=(f->window&1) ? f->data[f->window/2] : (f->data[f->window/2-1]+f->data[f->window/2])/2;
    return tmp;
}

// Insert a new sample 'value 'in the shift left register extracting & 
// remembering the oldest one.
void filter_shift(filter_t *f, int value){
    for(int i=0;i<f->window-1;i++)
        f->samples[i]=f->samples[i+1]; // shifts left
    f->samples[f->window-1]=value; // inserts the newest one head
}

// Removes allocated memory.
void filter_destroy(filter_t* f){
    if(f==NULL) return;
    free(f->data);
    free(f->samples);
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
