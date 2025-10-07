#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include "errorTypes.h"

#define POISON 0xEBA1
#define INCREASE_CAPACITY 2
#define PETUSHOK 0xDED

typedef int typeOfElement;       // если резко придется сменить тип и сделаем это в одно действие здесь

typedef struct
{
    unsigned firstPETUSHOK;

    typeOfElement* data;
    size_t capacity;
    size_t size;

    unsigned secondPETUSHOK;
} myStack_t;

errorType stackCtor(myStack_t* stk, size_t startingCapacity);
errorType stackDetor(myStack_t* stk);

errorType stackPush(myStack_t* stk, typeOfElement value);      // errorType
errorType stackPop(myStack_t* stk, typeOfElement* value);

errorType stackDump(myStack_t* stk, errorType err, const char* msg);
errorType stackVerificator(myStack_t* stk);
errorType reallocStack(myStack_t* stk);

#endif
