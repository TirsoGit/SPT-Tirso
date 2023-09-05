#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Place the capacity (number of decimal digits) to test (e.g. 3, 4 or 6).
#define TEST_CAPACITY 4

#if TEST_CAPACITY==3
    #define TEST_BCD_ITERATIONS 999
#elif TEST_CAPACITY==4
    #define TEST_BCD_ITERATIONS 9999
#elif TEST_CAPACITY==6
    #define TEST_BCD_ITERATIONS 999999
#endif

typedef uint32_t bcdDigits_t; // Holds up to 8-BCD digits
#define bcdDigitsLen (sizeof(bcdDigits_t)*2)

/* 
    BCD functions 
    
    These functions exec under the assumption that all BCD digits are valid,
    in any case you can call bcdIsValid() before to ensure that. Even when
    you can hold 'bcdDigitsLen', BCD digits operations are performed on 
    'bcdDigitsLen'-1 BCD digits.
*/

bool bcdIsValid(bcdDigits_t bcd){
    bcdDigits_t t1 = bcd + 0x06666666;
    bcdDigits_t t2 = bcd ^ 0x06666666;
    bcdDigits_t t3 = t1 ^ t2;
    bcdDigits_t t4 = t3 & 0x11111110;
    return (!t4);
}

bcdDigits_t bcdAdd(bcdDigits_t bcd1, bcdDigits_t bcd2){
    bcdDigits_t t1 = bcd1 + 0x06666666;
    bcdDigits_t t2 = t1 + bcd2;
    bcdDigits_t t3 = t1 ^ bcd2;
    bcdDigits_t t4 = t2 ^ t3;
    bcdDigits_t t5 = ~t4 & 0x11111110;
    bcdDigits_t t6 = (t5 >> 2) | (t5 >> 3);
    return (t2 - t6);
}

bcdDigits_t bcd10Complement(bcdDigits_t bcd){
    bcdDigits_t t1 = 0xf9999999 - bcd;
    return bcdAdd(t1, 0x00000001);
}

bcdDigits_t bcdSub(bcdDigits_t bcd1, bcdDigits_t bcd2){
    bcdDigits_t tenComp = bcd10Complement(bcd2);
    return (bcdAdd(bcd1,tenComp));
}

int bcdCompare(bcdDigits_t bcd1, bcdDigits_t bcd2, int nDigits){
    bcdDigits_t mask=0xf<<(nDigits*4);
    int result=0;
    for(int i=0;i<nDigits;i++){
        bcdDigits_t dig1=bcd1&mask;
        bcdDigits_t dig2=bcd2&mask;
        if(dig1>dig2){ result=1; break; }
        if(dig1<dig2){ result=-1; break; }
        mask>>=4;
    }
    return result;
}

bool bcdToArray(bcdDigits_t bcd, uint8_t* array){
    if(array==NULL) return false;
    for(int i=0;i<bcdDigitsLen;i++,bcd>>=4)
        array[i]=bcd&0xf;
    return true;
}

bool arrayToBcd(uint8_t* array, bcdDigits_t* bcd){
    if(array==NULL) return false;
    *bcd=0;
    for(int i=bcdDigitsLen-1;i>=0;i--){
        *bcd<<=4;
        *bcd|=(array[i]&0xf);
    }
    return true;
}

int cmpDigitsUp(const void* digit1, const void* digit2){
    int result=(*((uint8_t*)digit1)-*((uint8_t*)digit2));
    return (result>0)?1:((result<0)?-1:0);
}

int cmpDigitsDown(const void* digit1, const void* digit2){
    int result=(*((uint8_t*)digit1)-*((uint8_t*)digit2));
    return (result>0)?-1:((result<0)?1:0);
}

/*
    Sorts lower 'nDigits' BCD digits on 'bcd' to 'bcdSorted'. If 'direction' is 
    true in ascending order, or if it is false in descending order.
    
    THERE IS ROOM HERE FOR OPTIMIZATION!
*/
bool bcdSort(bcdDigits_t bcd, bcdDigits_t* bcdSorted, int nDigits, bool direction){
    if(bcdSorted==NULL) return false;
    uint8_t array[bcdDigitsLen];
    bcdToArray(bcd,array);
    qsort(array,nDigits,sizeof(uint8_t),direction?cmpDigitsUp:cmpDigitsDown);
    arrayToBcd(array,bcdSorted);
    return true;
}

uint8_t bcdGetDigit(bcdDigits_t bcd, int position){
    return (bcd>>position*4)&0xf;
}

bool bcdIsUniqueDigits(bcdDigits_t bcd, int nDigits){
    bool result=true;
    for(int i=0;i<nDigits-1;i++){
        uint8_t refDig=bcdGetDigit(bcd,i);
        for(int j=i+1;j<nDigits;j++){
            uint8_t nextDig=bcdGetDigit(bcd,j);
            if(refDig==nextDig) { result=false; break; }
        }
        if(!result) break;
    }
    return result;
}

bool bcdIsIdenticDigits(bcdDigits_t bcd, int nDigits){
    bool result=true;
    uint8_t refDig=bcdGetDigit(bcd,0);
    for(int i=1;i<nDigits;i++){
        uint8_t curDig=bcdGetDigit(bcd,i);
        if(curDig!=refDig){ result=false; break; }
    }
    return result;
}

/* 
    Transformation functions
    
    - Transformation step function executes just one transformation step under 
    the assumption that all BCD digits are valid, in any case, you can call 
    bcdIsValid() and bcdIsDistinticDigits() before to ensure that. Even when 
    you can hold 'bcdDigitsLen' BCD digits, operations are performed up to 
    'bcdDigitsLen'-1 BCD digits. Set 'capacity' equal to the number of digits 
    to transform.
    
    - Transformation function performs calls to transformation step function
    sequentially trying to find a fixed number.
*/

bcdDigits_t transformationStep(bcdDigits_t bcd, int capacity){
    if(!capacity || capacity>bcdDigitsLen-1) return (bcdDigits_t)0;
    /* 
        First step:
        Takes 4 decimal digits of the original number 'bcd', and arrange 
        it in ascending order, this will be the number A. 
    */
    bcdDigits_t numberA;
    bcdSort(bcd,&numberA,capacity,true/*ascending*/);
    /* 
        Second step:
        Arrange the numbers in descending order, we get the number B.
    */
    bcdDigits_t numberB;
    bcdSort(bcd,&numberB,capacity,false/*descending*/);
    /*
        Third step:
        Subtract the smaller one from the larger one, we get the number C. 
        This will be the result of our transformation.
    */
    bcdDigits_t numberC;
    int cmp=bcdCompare(numberA,numberB,capacity);
    if(cmp>0)
        numberC=bcdSub(numberA,numberB);
    else if(cmp<0)
        numberC=bcdSub(numberB,numberA);
    else
        numberC=0;
        
    return numberC; /*result of our transformation*/
}

int transformationFunction(bcdDigits_t bcd, int capacity, bcdDigits_t* fixedNumber){
    bcdDigits_t testedNumber=bcd;
    bcdDigits_t originalNumber=testedNumber;
    int iterationsCount=-1;
    for(int i=0;i<10;i++){
        bcdDigits_t transformedNumber = transformationStep(originalNumber,capacity);
        if(transformedNumber==originalNumber){ 
            *fixedNumber=transformedNumber; 
            iterationsCount=i; 
            break; 
        }
        
        originalNumber=transformedNumber;
    }
    return iterationsCount;
}

int main(void){
    printf("TESTING INTERESTING ARITHMETIC [DIGITS=%d]\n",TEST_CAPACITY);

    bcdDigits_t testedNumber, fixedNumber=0;
    int iterationsCount=-1;
    int testedCapacity=TEST_CAPACITY;
    int identicDigitsCounter=0;
    int successCounter=0;
    int errorCounter=0;
    int totalCounter=0;
    
    uint32_t iteratorCounters[100];
    memset(iteratorCounters,0,sizeof(iteratorCounters));
    uint32_t iteratorCountMax=0;
    bcdDigits_t fixedNumbers[100];
    memset(fixedNumbers,0,sizeof(fixedNumbers));
    uint32_t fixedNumberCounter=0;
    
    for(int i=0;i<=TEST_BCD_ITERATIONS;++i){
        testedNumber=bcdAdd(testedNumber,1);
        if(!bcdIsIdenticDigits(testedNumber,testedCapacity)){
            iterationsCount = transformationFunction(testedNumber, testedCapacity, &fixedNumber);
            if(iterationsCount<0){
                ++errorCounter;
            }
            else{
                ++successCounter;
                if(iterationsCount>iteratorCountMax)
                    iteratorCountMax=iterationsCount;
                if(iterationsCount<100)
                    ++iteratorCounters[iterationsCount];
                if(!fixedNumberCounter)
                    fixedNumbers[fixedNumberCounter++]=fixedNumber;
                else if(fixedNumbers[fixedNumberCounter-1]!=fixedNumber && fixedNumberCounter<100)
                    fixedNumbers[fixedNumberCounter++]=fixedNumber;
            }
        } else ++identicDigitsCounter;
        ++totalCounter;
    }
        
    printf("Total [%d] : Passed [%d] : Not Passed [%d] : Identic Digits [%d] : Fixed Numbers [%d] : Max Iterations [%d]",totalCounter,
                                                                                                                        successCounter,
                                                                                                                        errorCounter,
                                                                                                                        identicDigitsCounter,
                                                                                                                        fixedNumberCounter,
                                                                                                                        iteratorCountMax);
    for(int i=0;i<sizeof(iteratorCounters)/sizeof(uint32_t);i++)
        if(iteratorCounters[i])
            printf("\nOn iteration [%d] there are [%d] number(s) that converge.",i,iteratorCounters[i]);
    for(int i=0;i<fixedNumberCounter;i++)
        printf("\nFixed number [%d] with value [%x].",i,fixedNumbers[i]); 
        
    return 0;
}
