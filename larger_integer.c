#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Returns the larger integer (do not work when (a==0), (b==0) or (a==b)) */
int larger_int(int a, int b){
  return a*(bool)(a/b) + b*(bool)(b/a); 
}

/* Returns the larger integer (also works when (a==b) but not when (a==0) or (b==0)) */
int larger_int1(int a, int b){
  return (a - b) ? a*(bool)(a/b) + b*(bool)(b/a) : a; 
}

// Numbers to test (excluding 0)
int TEST_INTEGERS=20;

int main() {
    printf("TESTING LARGER INTEGER FUNCTION\n");
    
    for(int i=1;i<=TEST_INTEGERS-1;i++){
        int big=larger_int(i,TEST_INTEGERS-i);
        printf("Larger integer between [%d] vs. [%d] is [%d] <- [%s].\n",i,TEST_INTEGERS-i,big,
                                                                         ((big==i)||(big==(TEST_INTEGERS-i)))?"PASSED":"DO NOT PASSED!");
        big=larger_int1(i,TEST_INTEGERS-i);
        printf("Larger integer between [%d] vs. [%d] is [%d] <- [%s].\n",i,TEST_INTEGERS-i,big,
                                                                         ((big==i)||(big==(TEST_INTEGERS-i)))?"PASSED":"DO NOT PASSED!");
    }

    return 0;
}
