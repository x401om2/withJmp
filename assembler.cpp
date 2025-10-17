#include <stdio.h>
#include <string.h>
#include "processorOperations.h"
#include "assembler.h"
#include "commandConfig.h"


#define MAX_COMMANDS 1000




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

int readCommandsFromFile(FILE* file, char commands[][20], int maxCommands)
{
    char command[20] = "";
    int commandCount = 0;

    printf("=== ЧТЕНИЕ КОМАНД ===\n");
    while (fscanf(file, "%s", command) != EOF && commandCount < maxCommands)
    {
        strcpy(commands[commandCount], command);
        printf("Прочитана команда: '%s' (индекс: %d)\n", command, commandCount);
        commandCount++;
    }
    return commandCount;
}

void firstPassCollectLabels(char commands[][20], int commandCount, Label labels[])
{
    printf("\n=== ПЕРВЫЙ ПРОХОД ===\n");
    int currentAddress = 0;

    for (int i = 0; i < commandCount; i++)
    {
        printf("Обрабатываем: '%s' (адрес: %d)\n", commands[i], currentAddress);

        if (commands[i][0] == ':')
        {
            int labelNum = atoi(commands[i] + 1);
            if (labelNum >= 0 && labelNum < MAX_LABELS)
            {
                labels[labelNum].address = currentAddress;
                printf("Найдена метка :%d -> адрес %d\n", labelNum, currentAddress);
            }
        }
        else
        {
            int enumOfName = stringNameToEnum(commands[i]);

            if (enumOfName != -1 && commandHasArgument(enumOfName))
            {
                currentAddress += 2;
                i++;
                printf("Команда с аргументом '%s' -> новый адрес: %d\n", commands[i-1], currentAddress);
            }
            else if (enumOfName != -1)
            {
                currentAddress += 1;
                printf("Команда '%s' -> новый адрес: %d\n", commands[i], currentAddress);
            }
        }
    }
}

void printLabels(Label labels[])
{
    printf("\n=== НАЙДЕННЫЕ МЕТКИ ===\n");
    for (int i = 0; i < MAX_LABELS; i++)
    {
        if (labels[i].address != -1)
        {
            printf("Метка :%d -> адрес %d\n", i, labels[i].address);
        }
    }
}

int secondPassGenerateCode(char commands[][20], int commandCount, int pogrebuffer[], int pogrebufferSize, Label labels[])
{
    printf("\n=== ВТОРОЙ ПРОХОД ===\n");
    int countOfCommands = 0;

    for (int i = 0; i < commandCount && countOfCommands < pogrebufferSize; i++)
    {
        if (commands[i][0] == ':')
        {
            printf("Пропускаем метку: %s\n", commands[i]);
            continue;
        }

        int enumOfName = stringNameToEnum(commands[i]);
        printf("Обрабатываем команду: '%s' (enum=%d) на позиции %d\n", commands[i], enumOfName, countOfCommands);

        switch (enumOfName)
        {
            case CMD_PUSH:
                countOfCommands = handlePush(commands, &i, pogrebuffer, countOfCommands);
                break;

            case CMD_PUSHR:
                countOfCommands = handlePushR(commands, &i, pogrebuffer, countOfCommands);
                break;

            case CMD_POPR:
                countOfCommands = handlePopR(commands, &i, pogrebuffer, countOfCommands);
                break;

            case CMD_JMP:
            case CMD_JB:
            case CMD_JBE:
            case CMD_JE:
            case CMD_JNE:
            case CMD_JA:
            case CMD_JAE:
            case CMD_CALL:
                countOfCommands = processJumpCommandFromArray(enumOfName, commands, &i, labels, pogrebuffer, countOfCommands);
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
                printf("Неизвестная команда: %s\n", commands[i]);
                break;
        }
    }

    return countOfCommands;
}

int readCommandsToBuffer(FILE* assem_blya, int* pogrebuffer, int pogrebufferSize)
{
    char commands[MAX_COMMANDS][20] = {0};
    Label labels[MAX_LABELS] = {};

    // Инициализация меток
    for (int i = 0; i < MAX_LABELS; i++)
    {
        labels[i].address = -1;
    }

    // Чтение команд из файла
    int commandCount = readCommandsFromFile(assem_blya, commands, MAX_COMMANDS);

    // Первый проход - сбор меток
    firstPassCollectLabels(commands, commandCount, labels);

    // Печать найденных меток
    printLabels(labels);

    // Второй проход - генерация кода
    int result = secondPassGenerateCode(commands, commandCount, pogrebuffer, pogrebufferSize, labels);

    return result;
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
    if (strcmp(stringName, "call") == 0) return CMD_CALL;
    if (strcmp(stringName, "ret") == 0) return CMD_RET;

    return -1;
}

int processJumpCommandFromArray(int command, char commands[][20], int* currentIndex, Label labels[], int buffer[], int bufferPos)
{
    char labelName[20] = "";
    strcpy(labelName, commands[++(*currentIndex)]); // Берем следующий токен как метку

    // Пропускаем ':' в начале метки если есть
    const char* labelStr = (labelName[0] == ':') ? labelName + 1 : labelName;
    int labelNum = atoi(labelStr);

    buffer[bufferPos++] = command;

    if (labelNum >= 0 && labelNum < MAX_LABELS && labels[labelNum].address != -1)
    {
        buffer[bufferPos++] = labels[labelNum].address;
        printf("Записали команду %d с адресом перехода %d\n", command, labels[labelNum].address);
    }
    else
    {
        buffer[bufferPos++] = -1; // или обработка ошибки
        printf("Ошибка: метка :%d не найдена!\n", labelNum);
    }

    return bufferPos;
}


// массивы всего нахуй проверить чтобы без огромных перечислений - хуйня полная

// вместо switch указат на функцию и тд



int handlePush(char commands[][20], int* index, int buffer[], int bufferPos)
{
    int number = atoi(commands[++(*index)]);
    buffer[bufferPos++] = CMD_PUSH;
    buffer[bufferPos++] = number;
    printf("Записали PUSH %d\n", number);
    return bufferPos;
}

int handlePushR(char commands[][20], int* index, int buffer[], int bufferPos)
{
    char registerName[10] = "";
    strcpy(registerName, commands[++(*index)]);
    buffer[bufferPos++] = CMD_PUSHR;
    buffer[bufferPos++] = registerNameToIndex(registerName);
    printf("Записали PUSHR %s\n", registerName);
    return bufferPos;
}

int handlePopR(char commands[][20], int* index, int buffer[], int bufferPos)
{
    char registerName[10] = "";
    strcpy(registerName, commands[++(*index)]);
    buffer[bufferPos++] = CMD_POPR;
    buffer[bufferPos++] = registerNameToIndex(registerName);
    printf("Записали POPR %s\n", registerName);
    return bufferPos;
}

int handleSimpleCommand(int command, int buffer[], int bufferPos, const char* commandName)
{
    buffer[bufferPos++] = command;
    printf("Записали %s\n", commandName);
    return bufferPos;
}
