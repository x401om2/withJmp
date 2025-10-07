#include <stdio.h>
#include <string.h>
#include "processorOperations.h"
#include "assembler.h"

int translator()
{
    FILE* assemblya = fopen("assembler.txt", "r");

    int buffer[300] = {0};

    int countOfCommands = readCommandsToBuffer(assemblya, buffer, 300);

    fclose(assemblya);

    FILE* processor = fopen("processor.bin", "wb");

    fwrite(&buffer, sizeof(int), countOfCommands, processor);

    fclose(processor);

    printf("БИНАРНИК ЗЛОЕБУЧ\n");

    return 0;
}

int readCommandsToBuffer(FILE* assemblya, int* pogrebuffer, int pogrebufferSize)
{
    char command[20] = "";
    char registerName[10] = "";

    int number = 0;
    int countOfCommands = 0;

    while (fscanf(assemblya, "%s", command) != EOF && countOfCommands < pogrebufferSize)
    {
        if (strcmp(command, "push") == 0)
        {
            fscanf(assemblya, "%d", &number);
            pogrebuffer[countOfCommands++] = CMD_PUSH;
            pogrebuffer[countOfCommands++] = number;
        }
        else if (strcmp(command, "pushr") == 0)
        {
            fscanf(assemblya, "%s", registerName);
            int regIndex = registerNameToIndex(registerName);
            if (regIndex == -1) break;

            pogrebuffer[countOfCommands++] = CMD_PUSHR;
            pogrebuffer[countOfCommands++] = regIndex;
        }
        else if (strcmp(command, "popr") == 0)
        {
            fscanf(assemblya, "%s", registerName);
            int regIndex = registerNameToIndex(registerName);
            if (regIndex == -1) break;

            pogrebuffer[countOfCommands++] = CMD_POPR;
            pogrebuffer[countOfCommands++] = regIndex;
        }

        else if (strcmp(command, "jmp") == 0)
        {
            fscanf(assemblya, "%d", &number);
            pogrebuffer[countOfCommands++] = CMD_JMP;
            pogrebuffer[countOfCommands++] = number;
        }
        else if (strcmp(command, "add") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_ADD;
        }
        else if (strcmp(command, "sub") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_SUB;
        }
        else if (strcmp(command, "mul") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_MUL;
        }
        else if (strcmp(command, "div") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_DIV;
        }
        else if (strcmp(command, "out") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_OUT;
        }
        else if (strcmp(command, "sqrt") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_SQRT;
        }
        else if (strcmp(command, "in") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_IN;
        }
        else if (strcmp(command, "hlt") == 0)
        {
            pogrebuffer[countOfCommands++] = CMD_HLT;
            break;
        }
    }

    return countOfCommands;
}

int registerNameToIndex(const char* registerName)
{
    if (strcmp(registerName, "RAX") == 0) return REG_RAX;
    if (strcmp(registerName, "RBX") == 0) return REG_RBX;
    if (strcmp(registerName, "RCX") == 0) return REG_RCX;
    if (strcmp(registerName, "RDX") == 0) return REG_RDX;

    printf("Неизвестный регистр: %s\n", registerName);
    return -1;
}
