#include <stdio.h>
#include <stdint.h>

#define TEST_POWER2_ITERATIONS (1025)

// Fast method
//  returns 1 when x is a power of 2
unsigned testPower2Fast(unsigned x){
    return ((x & (x-1)) == 0);
}

// Shifting method
//  returns 1 when x is a power of 2
unsigned testPower2Shifting(unsigned x){
    if(!x) return 0; // 0 is not a power of 2 (return avoids an endless loop)
    while(!(x&1)) x>>=1;
    return ((x==1) ? 1 : 0);
}

int main()
{
    int result, resultFast;
    printf("TESTING POWER OF 2 [ITERATIONS=%d]\n",TEST_POWER2_ITERATIONS);
    for(unsigned i=1;i< TEST_POWER2_ITERATIONS;i++){
        resultFast=testPower2Fast(i);
        result=testPower2Shifting(i);
        printf("Testing [%u] : [%s] [%s].\n",i,!resultFast?"N":"Y",!result?"N":"Y");
    }
	    return 0;
}
