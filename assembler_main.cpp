#include <stdio.h>
#include <stdlib.h>
#include <stack.h>
#include <testingStack.h>
#include "assembler.h"
//TODO основные структуры создавать в мейне


int main()
{
    FILE* assemblya = fopen("factorial.txt", "r");

    const int maxCountOfCommands = 1000;

    int buffer[maxCountOfCommands] = {};

    int countOfCommands = readCommandsToBuffer(assemblya, buffer, maxCountOfCommands);

    fclose(assemblya);

    FILE* processor = fopen("processor.bin", "wb");

    fwrite(buffer, sizeof(int), countOfCommands, processor);

    fclose(processor);

    printf("БИНАРНИК ЗЛОЕБУЧ\n");

    return 0;
}
