
#include <stdio.h>
#include <math.h>
#include "stack.h"
#include "processorOperations.h"
#include "processor.h"
#include <unistd.h>

#define MAX_COMMANDS 300
#define COMMAND_DELAY_MS 1000

int processingFromFile(const char* filename, processor_t* processor)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("не открывает дверь баран %s\n", filename);
        return 1;
    }

    printf("\nначинаем педалировать с этого письмака: %s\n", filename);
    printf("=========================================================\n");

    processor -> code = (int*)calloc(MAX_COMMANDS, sizeof(int));
    processor -> sizeOfCode = 0;
    processor -> pointerOfInstruction = 0;

    while (processor->sizeOfCode < MAX_COMMANDS)
    {
        int rawCommand = 0;
        if (fread(&rawCommand, sizeof(int), 1, file) != 1)
        {
            break;
        }

        processor -> code[processor -> sizeOfCode] = rawCommand;                // в массив
        processor -> sizeOfCode++;

        if (rawCommand == CMD_PUSH || rawCommand == CMD_PUSHR ||
            rawCommand == CMD_POPR || rawCommand == CMD_JMP)
        {
            int value = 0;
            if (fread(&value, sizeof(int), 1, file) == 1)
            {
                processor->code[processor->sizeOfCode] = value;
                processor->sizeOfCode++;
            }
            else
            {
                break;
            }
        }

        if (rawCommand == CMD_HLT)
            break;
    }
    fclose(file);

    executeProcessor(processor);

    free(processor->code);
    stackDetor(&processor->calcStack);
    return 0;
}

void executeProcessor(processor_t* processor)
{
    while (processor->pointerOfInstruction < processor->sizeOfCode && processor->pointerOfInstruction >= 0)
    {
        int currentCommand = processor->code[processor->pointerOfInstruction];
        int commandSize = 1;

        if (currentCommand == CMD_PUSH || currentCommand == CMD_PUSHR || currentCommand == CMD_POPR || currentCommand == CMD_JMP)
        {
            commandSize = 2;
        }

        processCommand(processor, currentCommand);

        if (currentCommand != CMD_JMP)
        {
            processor->pointerOfInstruction += commandSize;
        }

        if (currentCommand == CMD_HLT)
            break;
    }
}

void processCommand(processor_t* processor, int command)
{
    switch (command)
    {
        case CMD_PUSH:
        {
            int value = processor->code[processor->pointerOfInstruction + 1];
            printf("PUSH %d\n", value);
            operationsOfProcessor(CMD_PUSH, value, &processor->calcStack, processor->registers);
            break;
        }

        case CMD_JMP:
        {
            int jumpTarget = processor->code[processor->pointerOfInstruction + 1];
            printf("JMP %d\n", jumpTarget);
            processor->pointerOfInstruction = jumpTarget;
            sleep(1);
            break;
        }

        case CMD_ADD:
            printf("ADD\n");
            operationsOfProcessor(CMD_ADD, 0, &processor->calcStack, processor->registers);
            break;

        case CMD_SUB:
            printf("SUB\n");
            operationsOfProcessor(CMD_SUB, 0, &processor->calcStack, processor->registers);
            break;

        case CMD_MUL:
            printf("MUL\n");
            operationsOfProcessor(CMD_MUL, 0, &processor->calcStack, processor->registers);
            break;

        case CMD_DIV:
            printf("DIV\n");
            operationsOfProcessor(CMD_DIV, 0, &processor->calcStack, processor->registers);
            break;

        case CMD_OUT:
            printf("OUT\n");
            operationsOfProcessor(CMD_OUT, 0, &processor->calcStack, processor->registers);
            break;

        case CMD_PUSHR:
        {
            int regIndex = processor->code[processor->pointerOfInstruction + 1];
            printf("PUSHR %s\n", getRegisterName(regIndex));
            operationsOfProcessor(CMD_PUSHR, regIndex, &processor->calcStack, processor->registers);
            break;
        }

        case CMD_POPR:
        {
            int regIndex = processor->code[processor->pointerOfInstruction + 1];
            printf("POPR %s\n", getRegisterName(regIndex));
            operationsOfProcessor(CMD_POPR, regIndex, &processor->calcStack, processor->registers);
            break;
        }

        case CMD_HLT:
            printf("HLT\n");
            break;

        default:
            printf("UNKNOWN COMMAND: %d\n", command);
            break;
    }
}


const char* getRegisterName(int regIndex)
{
    switch (regIndex)
    {
        case REG_RAX: return "RAX";
        case REG_RBX: return "RBX";
        case REG_RCX: return "RCX";
        case REG_RDX: return "RDX";
        default: return "UNKNOWN";
    }
}
