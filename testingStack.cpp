#include <stdio.h>
#include "testingStack.h"
#include "stack.h"
#include "errorTypes.h"


void testForFullStack(myStack_t* stack)
{
    for (int i = 0; i < 2; i++)
    {
        errorType result = stackPush(stack, 20);
        errorType err = stackVerificator(stack);
        if (result != ERROR_NO)
        {
            stackDump(stack, result, "STACK FULL - NO ELEMENTS ADDED");
            break;
        }
        stackDump(stack, err, "ADDING AN ELEMENT");
    }
}

void testForEmptyStack(myStack_t* stack)
{
    for (int i = 0; i < stack -> capacity; i++)
    {
        int value = 0;
        errorType result = stackPop(stack, &value);
        errorType err = stackVerificator(stack);

        if (result != ERROR_NO)
        {
            stackDump(stack, result, "СТЕК ПУСТОЙ - нет элементов");
            break;
        }
        stackDump(stack, err, "УДАЛЕНИЕ ЭЛЕМЕНТА");
    }
}

void funcSwitchErrors(errorType err) // for add cases if we need it
{
    switch(err) {
        case ERROR_NO:
            printf("stack status: OK\n");
            break;

        case EMPTY_STACK:
            printf("stack status: stack is empty\n");
            break;

        case NULL_POINTER_STACK:
            printf("stack status: null pointer to stack structure\n");
            break;

        case NULL_DATA_POINTER:
            printf("stack status: null pointer to stack data\n");
            break;

        case BAD_SIZE:
            printf("stack status: invalid size\n");
            break;

        case BAD_CAPACITY:
            printf("stack status: invalid stack capacity\n");
            break;

        case STACK_NO_MEMORY:
            printf("stack status: memory was not allocated\n");
            break;

        case SIZE_BIGGER_THAN_CAPACITY:
            printf("Stack status: size greater than capacity - incorrect\n");
            break;

        case PETUSHOK_V_BEDE:
            printf("Stack status: petuh damaged - memory corruption\n");
            break;

        case BAD_REALLOCATION:
            printf("Stack status: reallocation failed\n");
            break;

        default:
            printf("статус стека: неизвестная ошибка\n");
            break;
    }
}

void allTestsForBitches()
{
    myStack_t* testStackOne = NULL;
    myStack_t stk = {};

    size_t testCapacityZero = 0;
    size_t testCapacity = 2;

    printf("\n|==================TEST OF ZERO CAPACITY==================|\n");
    errorType firstBlood = stackCtor(&stk, testCapacityZero);
    stackDump(&stk, firstBlood, "BAD_CAPACITY TEST");

    printf("\n|==================TEST OF NULL POINTER STACK==================|\n");
    errorType secondBlood = stackCtor(testStackOne, 0);
    stackDump(testStackOne, secondBlood, "NULL_POINTER_STACK TEST");

    // printf("\n|==================TEST OF NULL DATA POINTER==================|\n");
    // errorType thirdBlood = stackCtor(&stk, testCapacity);
    // stk.data = NULL;
    // errorType dataError = stackVerificator(&stk);
    // stackDump(&stk, dataError, "NULL DATA POINTER");

    // printf("\n|==================TEST OF BAD SIZE==================|\n");
    // myStack_t stk4 = {};
    // stk4.size = -1;
    // errorType sizeError = stackVerificator(&stk4);
    // stackDump(&stk4, sizeError, "BAD_SIZE TEST");

    testSizeBiggerThanCapacity();

    testOfPetusharsMaboys();

    testForEmptyStack();
}

void simpleTestForFullAndEmptyStack()
{
    myStack_t stack = {};

    stackCtor(&stack, 3);
    stackPush(&stack, 10);
    stackPush(&stack, 20);

    stackDump(&stack, ERROR_NO, "AFTER ADD 2 ELEMENTS");


    testForFullStack(&stack);
    testForEmptyStack(&stack);


    stackPush(&stack, 11001);
    stackDump(&stack, ERROR_NO, "END OF TESTS");

    stackDetor(&stack);
}

void testOfPetusharsMaboys()
{
    printf("\n|==================TEST OF PETUSHOK PROBLEMS==================|\n");
    myStack_t stk6 = {};
    stackCtor(&stk6, 5);
    stk6.firstPETUSHOK = 0xDEAD;  // разьебали петуха первого
    errorType petushokError = stackVerificator(&stk6);
    stackDump(&stk6, petushokError, "PETUSHOK_V_BEDE TEST (ПЕРВЫЙ ГОЛУБОК) - СЛЕВА В ЗАЩИТЕ ПРОБОЙНА");

    // второй бебур
    myStack_t stk7 = {};
    stackCtor(&stk7, 5);
    stk7.secondPETUSHOK = 0xBEEF;  // разьебали второго петуха
    errorType petushokError2 = stackVerificator(&stk7);
    stackDump(&stk7, petushokError2, "PETUSHOK_V_BEDE TEST (ВТОРОЙ ГОЛУБОК) - СПРАВА В ЗАЩИТЕ ПРОБОЙНА ИДЕМ КО ДНУ");
    stackDetor(&stk7);
}

void testSizeBiggerThanCapacity()
{
    printf("\n|==================TEST OF SIZE BIGGER THAN CAPACITY==================|\n");
    myStack_t stk5 = {};
    stackCtor(&stk5, 3);
    stk5.size = 10;
    errorType sizeBiggerCapacityError = stackVerificator(&stk5);
    stackDump(&stk5, sizeBiggerCapacityError, "SIZE BIGGER THAN CAPACITY");
}

void testForEmptyStack()
{
    myStack_t stk8 = {};
    stackCtor(&stk8, 5);
    typeOfElement value = 0;

    errorType popErrorOfEmptyOpp = stackPop(&stk8, &value);
    stackDump(&stk8, popErrorOfEmptyOpp, "EMPTY_STACK TEST - не можем попкить отсюда ниччче");
    stackDetor(&stk8);
}
