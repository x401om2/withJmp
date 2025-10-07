#include <stdio.h>
#include "stack.h"                                          // для доступа к структуре и функциям в работе со стеком
#include <math.h>
#include "processorOperations.h"
#include <stdlib.h>

int operationsOfProcessor(commands_t command, double value, myStack_t* calcStack, int* registers)
{
    switch (command)
    {
        case CMD_PUSH:
            return stackPush(calcStack, (typeOfElement)value);

        case CMD_ADD:
            return add(calcStack);

        case CMD_SUB:
            return sub(calcStack);

        case CMD_MUL:
            return mul(calcStack);

        case CMD_DIV:
            return div(calcStack);

        case CMD_OUT:
            return out(calcStack);

        case CMD_SQRT:
            return sqRT(calcStack);

        case CMD_HLT:
            return 0;

        case CMD_PUSHR:
            return pushr(calcStack, registers, (int)value);

        case CMD_POPR:
            return popr(calcStack, registers, (int)value);

        case CMD_IN:
            return in(calcStack);

        case CMD_JMP:
            return 0;

        default:
            printf("ОТКУДА КОМАНДА????");
            return -1;
    }
}

int add(myStack_t* calcStack)
{
    typeOfElement val1, val2;

    if (calcStack -> size < 2)
    {
        printf("нужно 2 элемента чтобы ADD ");
        return -1;
    }
    stackPop(calcStack, &val2);
    stackPop(calcStack, &val1);

    return stackPush(calcStack, val1 + val2);
}

int sub(myStack_t* calcStack)
{
    typeOfElement val1, val2;

    if (calcStack -> size < 2)
    {
        printf("нужно 2 элемента чтобы SUB ");
        return -1;
    }
    stackPop(calcStack, &val2);
    stackPop(calcStack, &val1);
    return stackPush(calcStack, val1 - val2);
}

int sqRT(myStack_t* calcStack)
{
    typeOfElement sqrtValue;

    if (calcStack -> size < 1)
    {
        printf("НУЖНО ЗНАЧЕНИЕ ДЛЯ КОРНЯ ");
        return -1;
    }
    stackPop(calcStack, &sqrtValue);
    if (sqrtValue < 0)
    {
        printf("отрицат не можем взять корень ");
        stackPush(calcStack, sqrtValue);
        return -1;
    }
    return stackPush(calcStack, (typeOfElement)sqrt(sqrtValue));
}

int out(myStack_t* calcStack)
{
    printf("ТЕПЕРЬ СТЕК ВОТ ТАКОЙ[%lu]: ", calcStack -> size);
    for (size_t i = 0; i < calcStack -> size; i++)
    {
        printf("%d ", calcStack -> data[i]);
    }
    printf("\n");
    return 0;
}

int mul(myStack_t* calcStack)
{
    typeOfElement val1, val2;

    if (calcStack->size < 2)
    {
        printf("нужно 2 элемента чтобы MUL ");
        return -1;
    }
    stackPop(calcStack, &val2);
    stackPop(calcStack, &val1);
    return stackPush(calcStack, val1 * val2);
}

int div(myStack_t* calcStack)
{
    typeOfElement val1, val2;

    if (calcStack -> size < 2)
    {
        printf("нужно 2 элемента чтобы DIV ");
        return -1;
    }
    stackPop(calcStack, &val2);
    if (val2 == 0)
    {
        printf("на ноль делить ПОКА НЕЛЬЗЯ БРО ");
        stackPush(calcStack, val2);
        stackPush(calcStack, 0);
        return -1;
    }
    stackPop(calcStack, &val1);
    return stackPush(calcStack, val1 / val2);
}

int pushr(myStack_t* calcStack, int* registers, int regIndex)
{
    if (regIndex < 0 || regIndex >= 4)  // проверяем валидность индекса регистра
    {
        printf("Неверный индекс регистра: %d\n", regIndex);
        return -1;
    }
    return stackPush(calcStack, registers[regIndex]);
}

int popr(myStack_t* calcStack, int* registers, int regIndex)
{
    if (regIndex < 0 || regIndex >= 4)
    {
        printf("Неверный индекс регистра: %d\n", regIndex);
        return -1;
    }

    typeOfElement value;
    if (stackPop(calcStack, &value) != 0)
    {
        printf("Не могу извлечь из стека\n");
        return -1;
    }

    registers[regIndex] = value;
    return 0;
}

int in(myStack_t* calcStack)
{
    typeOfElement value = 0;
    printf("Введите число нормальное бля: ");

    if (scanf("%d", &value) != 1)
    {
        printf("Ошибка: иди нахуй\n");
        return -1;
    }
    return stackPush(calcStack, value);
}

int processorCtor(processor_t* processor)
{
    if (!processor) {
        printf("Ошибка: нулевой указатель на процессор\n");
        return -1;
    }

    if (stackCtor(&processor -> calcStack, 20) != 0) {
        printf("Ошибка инициализации стека\n");
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        processor -> registers[i] = 0;
    }

    printf("создали бля\n");
    return 0;
}

void processorDump(const processor_t* processor, const char* reason)
{
    if (!processor) {
        printf("DUMP: нулевой указатель\n");
        return;
    }

    printf("\n=== DUMP ПРОЦЕССОРА ===\n");
    printf("Причина: %s\n", reason);

    printf("Регистры:\n");
    printf("  RAX = %d\n", processor -> registers[0]);
    printf("  RBX = %d\n", processor -> registers[1]);
    printf("  RCX = %d\n", processor -> registers[2]);
    printf("  RDX = %d\n", processor -> registers[3]);

    printf("Стек [%lu]: ", processor -> calcStack.size);
    for (size_t i = 0; i < processor -> calcStack.size; i++) {
        printf("%d ", processor -> calcStack.data[i]);
    }
    printf("\n======================\n");
}

int processorVerify(const processor_t* processor)
{
    if (!processor) {
        printf("VERIFY: нулевой указатель\n");
        return -1;
    }

    if (!processor -> calcStack.data) {
        printf("VERIFY: стек не инициализирован\n");
        return -1;
    }

    if (processor -> calcStack.size > processor -> calcStack.capacity) {
        printf("VERIFY: переполнение стека\n");
        return -1;
    }

    printf("Процессор в норме\n");
    return 0;
}






int jmp(int targetCommand)
{
    return targetCommand;
}
