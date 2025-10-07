#ifndef ERROR_TYPES_H_
#define ERROR_TYPES_H_

typedef enum
{
    ERROR_NO = 0,
    EMPTY_STACK = 1,

    NULL_POINTER_STACK = 3,
    NULL_DATA_POINTER = 7,

    BAD_SIZE = 4,
    BAD_CAPACITY = 8,

    STACK_NO_MEMORY = 5,
    BAD_REALLOCATION = 6,

    SIZE_BIGGER_THAN_CAPACITY = 10,
    PETUSHOK_V_BEDE = 11

} errorType;

void funcSwitchErrors(errorType err);

#endif
