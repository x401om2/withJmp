#include "commandConfig.h"

// Определяем массив команд с аргументами
const int COMMAND_NEEDS_ARGUMENT[] = {
    [CMD_PUSH] = 1,
    [CMD_PUSHR] = 1,
    [CMD_POPR] = 1,
    [CMD_JMP] = 1,
    [CMD_JB] = 1,
    [CMD_JBE] = 1,
    [CMD_JE] = 1,
    [CMD_JNE] = 1,
    [CMD_JA] = 1,
    [CMD_JAE] = 1,
    [CMD_CALL] = 1
};

int commandHasArgument(int command)
{
    if (command >= 0 && command < sizeof(COMMAND_NEEDS_ARGUMENT)/sizeof(COMMAND_NEEDS_ARGUMENT[0]))
    {
        return COMMAND_NEEDS_ARGUMENT[command];
    }
    return 0;
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
