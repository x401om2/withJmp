#include <stdio.h>
#include <math.h>
#include "stack.h"
#include "processorOperations.h"
#include "processor.h"
#include "commandConfig.h"
#include <unistd.h>

#define MAX_COMMANDS 300

int processingFromFile(const char* filename, processor_t* processor)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("не открывает дверь баран %s\n", filename);
        return 1;
    }

    printf("=========================================================");
    printf("\nначинаем педалировать с этого письмака: %s\n", filename);
    printf("=========================================================\n");

    processor -> code = (int*)calloc(MAX_COMMANDS, sizeof(int));
    processor -> sizeOfCode = 0;
    processor -> pointerOfInstruction = 0;

    while (processor -> sizeOfCode < MAX_COMMANDS)
    {
        int rawCommand = 0;
        if (fread(&rawCommand, sizeof(int), 1, file) != 1)
        {
            break;
        }

        processor -> code[processor -> sizeOfCode] = rawCommand;                // в массив
        processor -> sizeOfCode++;

        if (commandHasArgument(rawCommand))
        {
            int value = 0;
            if (fread(&value, sizeof(int), 1, file) == 1)
            {
                processor -> code[processor -> sizeOfCode] = value;
                processor -> sizeOfCode++;
            }
            else
            {
                break;
            }
        }
    }
    fclose(file);

    executeProcessor(processor);

    free(processor -> code);
    stackDetor(&processor -> calcStack);
    return 0;
}

void executeProcessor(processor_t* processor)
{
    while (processor -> pointerOfInstruction < processor -> sizeOfCode && processor -> pointerOfInstruction >= 0)
    {
        int currentCommand = processor -> code[processor -> pointerOfInstruction];

        int commandSize = commandHasArgument(currentCommand) ? 2 : 1;

        processCommand(processor, currentCommand);

        if (shouldIncrementIP(currentCommand))
        {
            processor -> pointerOfInstruction += commandSize;
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
            int value = processor -> code[processor -> pointerOfInstruction + 1];
            // printf("PUSH %d\n", value);
            operationsOfProcessor(CMD_PUSH, value, &processor -> calcStack, processor -> registers, processor);
            break;
        }

        case CMD_JMP:
            jmp(processor);
            break;

        case CMD_JB:
            jb(&(processor -> calcStack), processor);
            break;

        case CMD_JBE:
            jbe(&(processor -> calcStack), processor);
            break;

        case CMD_JE:
            je(&(processor -> calcStack), processor);
            break;

        case CMD_JNE:
            jne(&(processor -> calcStack), processor);
            break;

        case CMD_JA:
            ja(&(processor -> calcStack), processor);
            break;

        case CMD_JAE:
            jae(&(processor -> calcStack), processor);
            break;

        case CMD_ADD:
            // printf("ADD\n");
            operationsOfProcessor(CMD_ADD, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_SUB:
            // printf("SUB\n");
            operationsOfProcessor(CMD_SUB, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_MUL:
            // printf("MUL\n");
            operationsOfProcessor(CMD_MUL, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_SQRT:
            // printf("SQRT\n");
            operationsOfProcessor(CMD_SQRT, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_DIV:
            // printf("DIV\n");
            operationsOfProcessor(CMD_DIV, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_IN:
            operationsOfProcessor(CMD_IN, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_OUT:
            // printf("OUT\n");
            operationsOfProcessor(CMD_OUT, 0, &processor -> calcStack, processor -> registers, processor);
            break;

        case CMD_PUSHR:
        {
            int regIndex = processor -> code[processor -> pointerOfInstruction + 1];
            // printf("PUSHR %s\n", getRegisterName(regIndex));
            operationsOfProcessor(CMD_PUSHR, regIndex, &processor -> calcStack, processor -> registers, processor);
            break;
        }

        case CMD_POPR:
        {
            int regIndex = processor -> code[processor -> pointerOfInstruction + 1];
            // printf("POPR %s\n", getRegisterName(regIndex));
            operationsOfProcessor(CMD_POPR, regIndex, &processor -> calcStack, processor -> registers, processor);
            break;
        }

        case CMD_CALL:
            call(processor);
            break;

        case CMD_RET:
            ret(processor);
            break;

        case CMD_HLT:
            // printf("HLT\n");
            break;

        default:
            // printf("UNKNOWN COMMAND: %d\n", command);
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
