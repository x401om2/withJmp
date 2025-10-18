#include <stdio.h>
#include <string.h>
#include "processorOperations.h"
#include "assembler.h"
#include "commandConfig.h"

//spu - виртуальный процессор

int readCommandsToBuffer(FILE* assem_blya, int* pogrebuffer, int pogrebufferSize)
{
    char** commands = (char**)calloc(maxCountOfCommands, sizeof(char*));
    Label labels[maxCountOfLabels] = {};

    initializeLabels(labels);

    int commandCount = readCommandsFromFile(assem_blya, commands, maxCountOfCommands);

    yuraGagarin(commands, commandCount, labels);

    printLabels(labels);

    int result = secondPassGenerateCode(commands, commandCount, pogrebuffer, pogrebufferSize, labels);

    for (int i = 0; i < commandCount; i++) {
        free(commands[i]);
    }
    free(commands);
    return result;
}

int readCommandsFromFile(FILE* file, char** commands, int maxCommands)                  // передаем массив указателей на указатели и заполняем его элементы-строки по каждому индексу
{
    char command[maxLenOfName] = {};

    int commandCount = 0;

    while (fscanf(file, "%s", command) != EOF && commandCount < maxCommands)
    {
        commands[commandCount] = (char*)calloc(strlen(command) + 1, sizeof(char));
        strcpy(commands[commandCount], command);                                            // заполняет каждую строку из массива
        commandCount++;
    }
    return commandCount;
}

void yuraGagarin(char** commands, int commandCount, Label* labels)
{
    printf("\n=== НАЧАЛО ГОВНА ===\n");
    int currentAddress = 0;                                                                 // адрес в машинном коде

    int labelIndex = 0;

    for (int indexOfCommand = 0; indexOfCommand < commandCount; indexOfCommand++)           // индекс в массиве с командами
    {
        printf("Обрабатываем: '%s' (адрес: %d)\n", commands[indexOfCommand], currentAddress);

        if (commands[indexOfCommand][0] == ':')
        {
            char* labelName = commands[indexOfCommand] + 1;                                     // сдвиг указ на символ сразу после ":"
            if (labelIndex < maxCountOfLabels)
            {
                strcpy(labels[labelIndex].name, labelName);
                labels[labelIndex].address = currentAddress;
                printf("Найдена метка :%s -> адрес %d\n", labelName, currentAddress);
                labelIndex++;
            }
        }
        else
        {
            int enumOfName = stringNameToEnum(commands[indexOfCommand]);
            if (enumOfName != -1 && commandHasArgument(enumOfName))
            {
                currentAddress += 2;
                indexOfCommand++;
                printf("Команда с аргументом '%s' -> новый адрес: %d\n", commands[indexOfCommand-1], currentAddress);
            }
            else if (enumOfName != -1)
            {
                currentAddress += 1;
                printf("Команда '%s' -> новый адрес: %d\n", commands[indexOfCommand], currentAddress);
            }
        }
    }
}

int secondPassGenerateCode(char** commands, int commandCount, int* pogrebuffer, int pogrebufferSize, Label* labels)
{
    printf("\n=== ВТОРОЙ ПРОХОД ===\n");
    int countOfCommands = 0;

    for (int indexInArrayOfCommandsAndArgs = 0; indexInArrayOfCommandsAndArgs < commandCount && countOfCommands < pogrebufferSize; indexInArrayOfCommandsAndArgs++) //ХУЙНЯ ПЕРЕДЕЛЫВАЙ i->index
    {
        if (commands[indexInArrayOfCommandsAndArgs][0] == ':')
        {
            printf("Пропускаем метку: %s\n", commands[indexInArrayOfCommandsAndArgs]);
            continue;
        }

        int enumOfName = stringNameToEnum(commands[indexInArrayOfCommandsAndArgs]);
        printf("Обрабатываем команду: '%s' (enum=%d) на позиции %d\n", commands[indexInArrayOfCommandsAndArgs], enumOfName, countOfCommands);

        switch (enumOfName)
        {
            case CMD_PUSH:
                countOfCommands = handlePush(commands, &indexInArrayOfCommandsAndArgs, pogrebuffer, countOfCommands);
                break;

            case CMD_PUSHR:
                countOfCommands = handlePushR(commands, &indexInArrayOfCommandsAndArgs, pogrebuffer, countOfCommands);
                break;

            case CMD_POPR:
                countOfCommands = handlePopR(commands, &indexInArrayOfCommandsAndArgs, pogrebuffer, countOfCommands);
                break;

            case CMD_JMP:
            case CMD_JB:
            case CMD_JBE:
            case CMD_JE:
            case CMD_JNE:
            case CMD_JA:
            case CMD_JAE:
            case CMD_CALL:
                countOfCommands = lebronJames(enumOfName, commands, &indexInArrayOfCommandsAndArgs, labels, pogrebuffer, countOfCommands);
                break;

            case CMD_RET:
                countOfCommands = handleSimpleCommand(CMD_RET, pogrebuffer, countOfCommands, "RET");
                break;

            case CMD_ADD:
                countOfCommands = handleSimpleCommand(CMD_ADD, pogrebuffer, countOfCommands, "ADD");
                break;

            case CMD_SUB:
                countOfCommands = handleSimpleCommand(CMD_SUB, pogrebuffer, countOfCommands, "SUB");
                break;

            case CMD_MUL:
                countOfCommands = handleSimpleCommand(CMD_MUL, pogrebuffer, countOfCommands, "MUL");
                break;

            case CMD_DIV:
                countOfCommands = handleSimpleCommand(CMD_DIV, pogrebuffer, countOfCommands, "DIV");
                break;

            case CMD_OUT:
                countOfCommands = handleSimpleCommand(CMD_OUT, pogrebuffer, countOfCommands, "OUT");
                break;

            case CMD_SQRT:
                countOfCommands = handleSimpleCommand(CMD_SQRT, pogrebuffer, countOfCommands, "SQRT");
                break;

            case CMD_IN:
                countOfCommands = handleSimpleCommand(CMD_IN, pogrebuffer, countOfCommands, "IN");
                break;

            case CMD_HLT:
                countOfCommands = handleSimpleCommand(CMD_HLT, pogrebuffer, countOfCommands, "HLT");
                break;

            default:
                printf("Неизвестная команда: %s\n", commands[indexInArrayOfCommandsAndArgs]);
                break;
        }
    }

    return countOfCommands;
}

int lebronJames(int enumOfName, char** commands, int* currentIndex, Label* labels, int* pogrebuffer, int positionInPogrebuffer)
{
    char labelName[maxLenOfName] = {};                                                      //20 to const int

    (*currentIndex)++;                                                                      // пропускаем 1 индекс чтобы указывать на аргумент функции лебрюна

    strcpy(labelName, commands[*currentIndex]);

    const char* labelStr = (labelName[0] == ':') ? labelName + 1 : labelName;               // Пропускаем ':' в начале метки если есть

    pogrebuffer[positionInPogrebuffer++] = enumOfName;

    int address = findLabelAddress(labels, labelStr);

    if (address != -1)
    {
        pogrebuffer[positionInPogrebuffer++] = address;
        printf("Записали команду %d с переходом на '%s' (адрес %d)\n", enumOfName, labelStr, address);
    }
    else
    {
        pogrebuffer[positionInPogrebuffer++] = -1;
        printf("Ошибка: метка '%s' не найдена!\n", labelStr);
    }

    return positionInPogrebuffer;
}
// массивы всего нахуй проверить чтобы без огромных перечислений - хуйня полная

int handlePush(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer)
{
    (*index)++;

    int number = atoi(commands[*index]);

    pogrebuffer[positionInPogrebuffer++] = CMD_PUSH;
    pogrebuffer[positionInPogrebuffer++] = number;
    printf("Записали PUSH %d\n", number);
    return positionInPogrebuffer;
}

int handlePushR(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer)
{
    char registerName[maxLenOfName] = {};
    (*index)++;
    strcpy(registerName, commands[*index]);

    pogrebuffer[positionInPogrebuffer++] = CMD_PUSHR;
    pogrebuffer[positionInPogrebuffer++] = registerNameToIndex(registerName);
    printf("Записали PUSHR %s\n", registerName);
    return positionInPogrebuffer;
}

int handlePopR(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer)
{
    char registerName[maxLenOfName] = {};
    (*index)++;                                                                 //чтобы указ на то - что после команды
    strcpy(registerName, commands[*index]);

    pogrebuffer[positionInPogrebuffer++] = CMD_POPR;
    pogrebuffer[positionInPogrebuffer++] = registerNameToIndex(registerName);
    printf("Записали POPR %s\n", registerName);
    return positionInPogrebuffer;
}

int handleSimpleCommand(int command, int* pogrebuffer, int positionInPogrebuffer, const char* commandName)
{
    pogrebuffer[positionInPogrebuffer++] = command;
    printf("Записали %s\n", commandName);
    return positionInPogrebuffer;
}

int findLabelAddress(Label* labels, const char* labelName)
{
    for (int i = 0; i < maxCountOfLabels; i++)
    {
        if (labels[i].address != -1 && strcmp(labels[i].name, labelName) == 0)
        {
            return labels[i].address;
        }
    }
    return -1;
}

void printLabels(Label* labels)
{
    printf("\n=== НАЙДЕННЫЕ МЕТКИ ===\n");
    for (int i = 0; i < maxCountOfLabels; i++)
    {
        if (labels[i].address != -1)
            printf("Метка :%s -> адрес %d\n", labels[i].name, labels[i].address);
    }
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

int stringNameToEnum(const char* stringName)
{
    if (strcmp(stringName, "push")  == 0) return CMD_PUSH;
    if (strcmp(stringName, "pushr") == 0) return CMD_PUSHR;
    if (strcmp(stringName, "popr")  == 0) return CMD_POPR;
    if (strcmp(stringName, "jmp")   == 0) return CMD_JMP;
    if (strcmp(stringName, "jb")    == 0) return CMD_JB;
    if (strcmp(stringName, "jbe")   == 0) return CMD_JBE;
    if (strcmp(stringName, "je")    == 0) return CMD_JE;
    if (strcmp(stringName, "jne")   == 0) return CMD_JNE;
    if (strcmp(stringName, "ja")    == 0) return CMD_JA;
    if (strcmp(stringName, "jae")   == 0) return CMD_JAE;
    if (strcmp(stringName, "add")   == 0) return CMD_ADD;
    if (strcmp(stringName, "sub")   == 0) return CMD_SUB;
    if (strcmp(stringName, "mul")   == 0) return CMD_MUL;
    if (strcmp(stringName, "div")   == 0) return CMD_DIV;
    if (strcmp(stringName, "out")   == 0) return CMD_OUT;
    if (strcmp(stringName, "sqrt")  == 0) return CMD_SQRT;
    if (strcmp(stringName, "in")    == 0) return CMD_IN;
    if (strcmp(stringName, "hlt")   == 0) return CMD_HLT;
    if (strcmp(stringName, "call")  == 0) return CMD_CALL;
    if (strcmp(stringName, "ret")   == 0) return CMD_RET;

    return -1;
}

void initializeLabels(Label* labels)
{
    for (int i = 0; i < maxCountOfLabels; i++)
    {
        labels[i].name[0] = '\0';                                                   // пустая строка
        labels[i].address = -1;                                                     // невалидный адрес
    }
}
