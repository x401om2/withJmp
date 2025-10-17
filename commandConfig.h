#ifndef COMMAND_CONFIG_H
#define COMMAND_CONFIG_H

#include "processorOperations.h"

// Массив команд, которые требуют аргумент (1 - требует, 0 - нет)
extern const int COMMAND_NEEDS_ARGUMENT[];

// Функция для проверки нужен ли команде аргумент
int commandHasArgument(int command);

int shouldIncrementIP(int command);

#endif
