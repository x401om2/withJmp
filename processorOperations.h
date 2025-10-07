#ifndef CALCULATOR_OPERATIONS_H_
#define CALCULATOR_OPERATIONS_H_

#include "stack.h"
#include <stdio.h>
#include <stdbool.h>

#define REG_RAX 0
#define REG_RBX 1
#define REG_RCX 2
#define REG_RDX 3


typedef enum
{
    CMD_NOTHING = 0,
    CMD_PUSH = 1,   // добавить в стек
    CMD_ADD = 2,    // сложить
    CMD_SUB = 3,    // вычитание
    CMD_MUL = 4,    // умножение
    CMD_DIV = 5,    // деление
    CMD_OUT = 6,    // вывод результата
    CMD_SQRT = 7,   // корень
    CMD_HLT = 9,
    CMD_PUSHR = 10,
    CMD_POPR = 11,
    CMD_JMP = 12,
    CMD_IN = 95

} commands_t;

typedef struct {

    commands_t cmd;
    int value;
    bool hasValue;

} commandForBinary_t;       // команда - значение (число или индекс регистра) - статус значения

typedef struct
{

    myStack_t calcStack;
    int* code;
    int sizeOfCode;
    int pointerOfInstruction;
    int registers[4];

} processor_t;              // стек - массив с кодом машинным - размер массива - указатель на текущ исполняемую инструкцию - массив регистров

int operationsOfProcessor(commands_t command, double value, myStack_t* calcStack, int* registers);

int pushr(myStack_t* calcStack, int* registers, int regIndex);
int popr(myStack_t* calcStack, int* registers, int regIndex);

int add(myStack_t* calcStack);
int sub(myStack_t* calcStack);
int div(myStack_t* calcStack);
int mul(myStack_t* calcStack);
int sqRT(myStack_t* calcStack);

int out(myStack_t* calcStack);
int in(myStack_t* calcStack);

int jmp(int targetCommand);


int processorCtor(processor_t* processor);
// int processorDetor(processor_t* proc);
void processorDump(const processor_t* processor, const char* reason);
int processorVerify(const processor_t* processor);

#endif
