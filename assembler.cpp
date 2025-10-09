#include <stdio.h>
#include <string.h>
#include "processorOperations.h"
#include "assembler.h"
//TODO main можно сразу
// просто перекинуть внутрянку

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
    int enumOfName = 0;

    while (fscanf(assemblya, "%s", command) != EOF && countOfCommands < pogrebufferSize)
    {
        enumOfName = stringNameToEnum(command);
        switch (enumOfName)
        {
            case CMD_PUSH:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_PUSH;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_PUSHR:
            {
                fscanf(assemblya, "%s", registerName);
                int regIndexPush = registerNameToIndex(registerName);
                if (regIndexPush == -1) break;
                pogrebuffer[countOfCommands++] = CMD_PUSHR;
                pogrebuffer[countOfCommands++] = regIndexPush;
                break;
            }
            case CMD_POPR:
            {
                fscanf(assemblya, "%s", registerName);
                int regIndexPop = registerNameToIndex(registerName);
                if (regIndexPop == -1) break;
                pogrebuffer[countOfCommands++] = CMD_POPR;
                pogrebuffer[countOfCommands++] = regIndexPop;
                break;
            }
            case CMD_JMP:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JMP;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JB:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JB;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JBE:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JBE;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JE:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JE;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JNE:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JNE;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JA:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JA;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_JAE:
                fscanf(assemblya, "%d", &number);
                pogrebuffer[countOfCommands++] = CMD_JAE;
                pogrebuffer[countOfCommands++] = number;
                break;

            case CMD_ADD:
                pogrebuffer[countOfCommands++] = CMD_ADD;
                break;

            case CMD_SUB:
                pogrebuffer[countOfCommands++] = CMD_SUB;
                break;

            case CMD_MUL:
                pogrebuffer[countOfCommands++] = CMD_MUL;
                break;

            case CMD_DIV:
                pogrebuffer[countOfCommands++] = CMD_DIV;
                break;

            case CMD_OUT:
                pogrebuffer[countOfCommands++] = CMD_OUT;
                break;

            case CMD_SQRT:
                pogrebuffer[countOfCommands++] = CMD_SQRT;
                break;

            case CMD_IN:
                pogrebuffer[countOfCommands++] = CMD_IN;
                break;

            case CMD_HLT:
                pogrebuffer[countOfCommands++] = CMD_HLT;
                break;

            default:
                printf("Неизвестная команда: %s\n", command);
                break;
        }

        if (enumOfName == CMD_HLT) break;
    }

    return countOfCommands;
}

// бля мне самому противно стопроцентный свитч????
int registerNameToIndex(const char* registerName)
{
    if (strcmp(registerName, "RAX") == 0) return REG_RAX;
    if (strcmp(registerName, "RBX") == 0) return REG_RBX;
    if (strcmp(registerName, "RCX") == 0) return REG_RCX;
    if (strcmp(registerName, "RDX") == 0) return REG_RDX;

    printf("Неизвестный регистр: %s\n", registerName);
    return -1;
}

int stringNameToEnum(const char* stringName)
{
    if (strcmp(stringName, "push") == 0) return CMD_PUSH;
    if (strcmp(stringName, "pushr") == 0) return CMD_PUSHR;
    if (strcmp(stringName, "popr") == 0) return CMD_POPR;
    if (strcmp(stringName, "jmp") == 0) return CMD_JMP;
    if (strcmp(stringName, "jb") == 0) return CMD_JB;
    if (strcmp(stringName, "jbe") == 0) return CMD_JBE;
    if (strcmp(stringName, "je") == 0) return CMD_JE;
    if (strcmp(stringName, "jne") == 0) return CMD_JNE;
    if (strcmp(stringName, "ja") == 0) return CMD_JA;
    if (strcmp(stringName, "jae") == 0) return CMD_JAE;
    if (strcmp(stringName, "add") == 0) return CMD_ADD;
    if (strcmp(stringName, "sub") == 0) return CMD_SUB;
    if (strcmp(stringName, "mul") == 0) return CMD_MUL;
    if (strcmp(stringName, "div") == 0) return CMD_DIV;
    if (strcmp(stringName, "out") == 0) return CMD_OUT;
    if (strcmp(stringName, "sqrt") == 0) return CMD_SQRT;
    if (strcmp(stringName, "in") == 0) return CMD_IN;
    if (strcmp(stringName, "hlt") == 0) return CMD_HLT;

    return -1;
}
