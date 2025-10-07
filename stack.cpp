#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "errorTypes.h"
#include "stack.h"
#include "testingStack.h"


#define STACK_CHECK_AND_RETURN(stk, msg) \
    do { \
        errorType resultOfCheck = stackVerificator(stk); \
        if (resultOfCheck != ERROR_NO) { \
            stackDump(stk, resultOfCheck, msg); \
            return resultOfCheck; \
        } \
    } while(0)

// TODO задефайнить проверку стека ВЕЗДЕ и внести проверку s > c добавить просто реаллокацию +
// TODO ассерты закинуть

errorType stackPush(myStack_t* stk, typeOfElement value)
{
    STACK_CHECK_AND_RETURN(stk, "стек сломан до добавления\n");

    assert(stk != NULL); // на всякий случай воткнул

    if (stk -> size == stk -> capacity)
    {
        errorType resultOfRealloc = reallocStack(stk);
        if (resultOfRealloc != ERROR_NO)
        {
            stackDump(stk, resultOfRealloc, "реалокация отсосала не вышло не фартануло\n");
            return resultOfRealloc;
        }
    }

    stk -> data[stk -> size] = value;
    (stk -> size)++;

    STACK_CHECK_AND_RETURN(stk, "стек сломан после добавления\n");

    return ERROR_NO;
}

errorType stackPop(myStack_t* stk, typeOfElement* value)
{
    assert(value != NULL);

    STACK_CHECK_AND_RETURN(stk, "стек сломан до добавления\n");

    if (stk -> size == 0)
    {
        return EMPTY_STACK;
    }

    stk -> size--;
    *value = stk -> data[stk -> size];
    stk -> data[stk -> size] = POISON;

    STACK_CHECK_AND_RETURN(stk, "стек сломан после добавления\n");

    return ERROR_NO;
}

// TODO канарейка вначале и в конце + вроде БЫЫ

errorType stackCtor(myStack_t* stk, size_t startingCapacity)
{
    // assert(stk != NULL);
    // assert(startingCapacity > 0); - скипнет программу

    if (stk == NULL)
        return NULL_POINTER_STACK;

    if (startingCapacity <= 0)
        return BAD_CAPACITY;

    stk -> firstPETUSHOK = PETUSHOK;
    stk -> secondPETUSHOK = PETUSHOK;

    stk -> data = (typeOfElement*) calloc(startingCapacity, sizeof(typeOfElement));

    // assert(stk->data != NULL); - прогу скипнет

    if (stk -> data == NULL)
        return NULL_DATA_POINTER;

    for (size_t i = 0; i < startingCapacity; i++)
    {
        stk -> data[i] = POISON;
    }

    stk -> capacity = startingCapacity;
    stk -> size = 0;

    return ERROR_NO;
}

errorType stackDetor(myStack_t* stk)
{
    if (stk == NULL)
        return NULL_POINTER_STACK;

    free(stk -> data);

    stk -> data = NULL;
    stk -> capacity = 0;
    stk -> size = 0;

    return ERROR_NO;
}

errorType stackDump(myStack_t* stk, errorType err, const char* msg) // третий параметр - выводящийся текст ошибки и проверки
{
    assert(msg != NULL);

    printf("\n--------------------------------THE NEW DUMPY BITCHES--------------------------------\n");
    printf("\nMessage: %s\n", msg);
    printf("Error code: %d\n", err);
    printf("Stack address: %p\n", stk);

    if (stk == NULL)
    {
        printf("\n--------------------------------THE END DUMPY BITCHES--------------------------------\n");
        return NULL_POINTER_STACK;
    }

    printf("\nPETUH PERED DVER'U: %X (ожидаемо: %X)\n", stk -> firstPETUSHOK, PETUSHOK);

    printf("\nPETUH ZA DVER'U: %X (ожидаемо: %X)\n\n", stk -> secondPETUSHOK, PETUSHOK);

    if (stk -> firstPETUSHOK != PETUSHOK || stk -> secondPETUSHOK != PETUSHOK) {
        printf("\n=====PETUSHOK V BEDE=====\n\n");
    }

    funcSwitchErrors(err); // print stack status

    printf("size: %lu\n", stk -> size);
    printf("capacity: %lu\n", stk -> capacity);
    printf("data pointer: %p\n", stk -> data);

    errorType result = stackVerificator(stk);

    if(result == ERROR_NO)          //  stk -> data != NULL && stk -> capacity > 0 &&
    {
        printf("Data contents:\n");
        for(size_t i = 0; i < stk -> capacity; i++)
        {
            if(i < stk -> size)
            {
                printf("  *[%lu] = %d (active)\n", i, stk -> data[i]);
            } else {
                printf("  [%lu] = %X (POISON)\n", i, stk -> data[i]);
            }
        }
    }else {
        return result;
    }
    printf("\n--------------------------------THE END DUMPY BITCHES--------------------------------\n");
    return ERROR_NO;
}

errorType stackVerificator(myStack_t* stk)
{
    if (stk == NULL)
    {
        return NULL_POINTER_STACK;
    }
    if (stk -> size < 0)
    {
        return BAD_SIZE;
    }
    if (stk -> data == NULL)
    {
        return NULL_DATA_POINTER;
    }
    if (stk -> capacity <= 0)
    {
        return BAD_CAPACITY;
    }
    if (stk -> size > stk -> capacity)
    {
        return SIZE_BIGGER_THAN_CAPACITY;
    }
    if (stk -> firstPETUSHOK != PETUSHOK || stk -> secondPETUSHOK != PETUSHOK)
    {
        return PETUSHOK_V_BEDE;
    }
    return ERROR_NO;
}

errorType reallocStack(myStack_t* stk)
{
    size_t newCapacity = (stk -> capacity) * INCREASE_CAPACITY;

    typeOfElement* newData = (typeOfElement*) realloc(stk -> data, newCapacity * sizeof(typeOfElement));

    stk -> data = newData;

    for (size_t i = stk -> capacity; i < newCapacity; i++)
    {
        stk -> data[i] = POISON;
    }

    stk -> capacity = newCapacity;

    return ERROR_NO;
}
