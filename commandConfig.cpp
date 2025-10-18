#include "commandConfig.h"

// Определяем массив команд с аргументами
int commandHasArgument(int command)
{
    switch (command)
    {
        case CMD_PUSH:
        case CMD_PUSHR:
        case CMD_POPR:
        case CMD_JMP:
        case CMD_JB:
        case CMD_JBE:
        case CMD_JE:
        case CMD_JNE:
        case CMD_JA:
        case CMD_JAE:
        case CMD_CALL:
            return 1;
        default:
            return 0;
    }
}

int shouldIncrementIP(int command)
{
    switch (command)
    {
        case CMD_JMP:
        case CMD_JB:
        case CMD_JBE:
        case CMD_JE:
        case CMD_JNE:
        case CMD_JA:
        case CMD_JAE:
        case CMD_CALL:
        case CMD_RET:
        case CMD_HLT:
            return 0; // не увеличивать IP
        default:
            return 1; // увеличивать IP
    }
}
