#include <stdio.h>

// Place the number  of iterations to test.
#define TEST_ITERATIONS     20  
// Place an exclusive '1' on test you want to perform.
#define TEST_SPT_EXERCISE   0   
#define TEST_FACTORIAL      1
// Define a name to the test.
#define TEST_NAME_SPT       "SPT TEST"  
#define TEST_NAME_FACTORIAL "FACTORIAL"
// Associate a name to a test.
#if TEST_SPT_EXERCISE
    #define TEST_NAME TEST_NAME_SPT
#elif TEST_FACTORIAL
    #define TEST_NAME TEST_NAME_FACTORIAL
#endif

unsigned fn(unsigned k)
{
    if (k == 0 || k == 1)
    {
	    return 1;
    }
    else
    {
	    return fn(k - 1) + fn(k - 2);
    }
}

unsigned fnIterative(unsigned k){
  unsigned varfnk_1=1, varfnk_2=1;
  for (unsigned i=2; i<=k; ++i)
  {
    unsigned result=varfnk_1+varfnk_2;
    varfnk_1=varfnk_2;
    varfnk_2=result;
  }
  return varfnk_2;
}  

unsigned factorial(unsigned n) {
    if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

unsigned factorialIterative(unsigned n) {
    unsigned varfactorialn_1=1;
    for(unsigned i=1;i<=n;i++){
        unsigned result=i*varfactorialn_1;
        varfactorialn_1=result;
    }
    return varfactorialn_1;
}

int main(void){
    // Your code here!
    printf("TESTING RECURSIVE VS. ITERATIVE [ALGORITHM=>%s]\n",TEST_NAME);
    unsigned errorCnt=0;
    for(unsigned i=0;i<TEST_ITERATIONS;i++)
    {
        #if TEST_SPT_EXERCISE
            unsigned rtmp=fn(i);
            unsigned itmp=fnIterative(i);
        #elif TEST_FACTORIAL
            unsigned rtmp=factorial(i);
            unsigned itmp=factorialIterative(i);        
        #endif
        if(rtmp==itmp) 
            printf("For k=%u [result=%u] => MATCH.\n",i,rtmp);
        else{
            printf("For k=%u [recursive=%u vs iterative=%u] => DO NOT MATCH!\n",i,rtmp,itmp);
            ++errorCnt;
        }
    }
    printf("\nTESTINGS [%s] : [ERRORS=%u].\n",(!errorCnt)?"PASSED":"NOT PASSED!",errorCnt);
}

