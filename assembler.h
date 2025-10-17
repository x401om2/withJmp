#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "processorOperations.h"

int translator();
int readCommandsToBuffer(FILE* assem_blya, int* pogrebuffer, int pogrebufferSize);
int secondPassGenerateCode(char commands[][20], int commandCount, int pogrebuffer[], int pogrebufferSize, Label labels[]);
void printLabels(Label labels[]);
void firstPassCollectLabels(char commands[][20], int commandCount, Label labels[]);


int registerNameToIndex(const char* registerName);
int stringNameToEnum(const char* stringName);

int processJumpCommandFromArray(int command, char commands[][20], int* currentIndex, Label labels[], int buffer[], int bufferPos);



int handleSimpleCommand(int command, int buffer[], int bufferPos, const char* commandName);
int handlePopR(char commands[][20], int* index, int buffer[], int bufferPos);
int handlePushR(char commands[][20], int* index, int buffer[], int bufferPos);
int handlePush(char commands[][20], int* index, int buffer[], int bufferPos);


#endif
