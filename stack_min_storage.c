#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST_STACK_ITERATIONS (10)

#define stackCapacity  (100000L)
#define stackEmpty     (-1L)
#define stackIsEmpty() (p_stack->top==stackEmpty)
#define stackIsFull()  (p_stack->top>=stackCapacity)

// Stack type definition.
typedef struct {
    long top;
    int *data;
} t_stack;

// Initializes stack (reserving the storage of the minimum element).
bool init(t_stack* p_stack){
    if(p_stack==NULL) return false; // invalid stack pointer!
    p_stack->top = stackEmpty;
    p_stack->data = (int*)malloc(sizeof(int)); // mallocs just one element for the stack. 
    return (p_stack->data!=NULL); // true if sucess, false if no enough memory.
}

// Frees stack (function renamed to sfree to avoid name conflicts with standard C function free).
void sfree(t_stack* p_stack){
    if(p_stack==NULL) return; // invalid stack pointer!
    if(p_stack->data!=NULL)
        free((void*)p_stack->data);
}

// Pushs a new value to the top of stack.
bool push(t_stack* p_stack, int val) {
    if(p_stack==NULL) return false; // invalid stack pointer!
    if(stackIsFull()) return false; // stack is full!
    p_stack->data[++p_stack->top] = val; // save value on the previous created space
    // Reallocates data to a new space reserving more space to allocate the next pushed element
    p_stack->data=realloc(p_stack->data,(p_stack->top+2)*sizeof(int));
    return true;
}

// Pops the value on top of stack.
int pop(t_stack* p_stack, int* p_val) {
    if(p_stack==NULL) return false; // invalid stack pointer!
    if(stackIsEmpty()) return false; // stack is empty!
    *p_val = p_stack->data[p_stack->top--];
    // Reallocates data to a new space releasing the space of the popped element
    p_stack->data=realloc(p_stack->data,(p_stack->top+1)*sizeof(int));
    return true;
}

// Gets the maximum value stored in the stack.
int get_max(t_stack* p_stack, int* p_val) {
    if(p_stack==NULL) return false; // invalid stack pointer!
    if(stackIsEmpty()) return false; // stack is empty!
    int max=p_stack->data[p_stack->top];
    for(int i=p_stack->top;i>=0;i--)
        if(p_stack->data[i]>max)
            max=p_stack->data[i];
    *p_val=max;
    return true;
}

// Array for testing purpose only!
int TEST_STACK_ARRAY[TEST_STACK_ITERATIONS*2];

int main() {
    t_stack oneStack;
    
    printf("TESTING MIN. STORAGE STACK FUNCTIONS [ITERATIONS=%d]\n",TEST_STACK_ITERATIONS);
    /* init */
    printf("Testing stack [init() function] %s.\n",init(&oneStack)?"PASSED":"NOT PASSED!");
    /* push */
    unsigned i;
    for(i=0;i<TEST_STACK_ITERATIONS;i++){
        int tmp=rand();
        push(&oneStack,tmp);
        printf("Pushing [%d] : [value=%d].\n",i,tmp);
        TEST_STACK_ARRAY[i]=tmp; // <<== Test line.
    }
    /* get_max */
    int max;
    get_max(&oneStack,&max);
    printf("Testing stack [get_max() function] detected maximum is [%d].\n",max);
    /* pop */
    int val;
    i=TEST_STACK_ITERATIONS;
    while(pop(&oneStack,&val)){
        printf("Poping [%d] : [value=%d].\n",TEST_STACK_ITERATIONS*2-(i+1),val);
        TEST_STACK_ARRAY[i++]=val; // <<== Test line.
    }
    /* free */
    sfree(&oneStack);
    
    /* testing pushing-poping order */
    for(i=0;i<TEST_STACK_ITERATIONS;i++){
        printf("Testing ");
        if(TEST_STACK_ARRAY[i]==TEST_STACK_ARRAY[(TEST_STACK_ITERATIONS*2-1)-i])
            printf("PUSH [%d] vs POP [%d] with value [%d] ==> PASSED.\n", i, i,TEST_STACK_ARRAY[i]);
        else
            printf("PUSH [%d] vs POP [%d] with value [%d] ==> NOT PASSED.\n", i, i,TEST_STACK_ARRAY[i]);
    }
    return 0;
}
