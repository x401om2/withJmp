#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include "processorOperations.h"



const int maxLenOfName = 20;
const int maxCountOfCommands = 1000;
const int maxCountOfLabels = 10;

int readCommandsToBuffer(FILE* assem_blya, int* pogrebuffer, int pogrebufferSize);

int readCommandsFromFile(FILE* file, char** commands, int maxCommands);

int secondPassGenerateCode(char** commands, int commandCount, int* pogrebuffer, int pogrebufferSize, Label* labels); //ХУЙНЯ ПЕРЕДЕЛЫВАЙ -

void printLabels(Label* labels);

void yuraGagarin(char** commands, int commandCount, Label* labels); //ХУЙНЯ ПЕРЕДЕЛЫВАЙ -

int findLabelAddress(Label* labels, const char* labelName);

int registerNameToIndex(const char* registerName);
int stringNameToEnum(const char* stringName);

int lebronJames(int enumOfName, char** commands, int* currentIndex, Label* labels, int* pogrebuffer, int positionInPogrebuffer); //ХУЙНЯ ПЕРЕДЕЛЫВАЙ -

int handleSimpleCommand(int command, int* pogrebuffer, int positionInPogrebuffer, const char* commandName);
int handlePopR(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer);
int handlePushR(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer);
int handlePush(char** commands, int* index, int* pogrebuffer, int positionInPogrebuffer);

void initializeLabels(Label* labels);


#endif
