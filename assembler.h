#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

int translator();
int readCommandsToBuffer(FILE* assemblya, int* pogrebuffer, int pogrebufferSize);

int registerNameToIndex(const char* registerName);
int stringNameToEnum(const char* stringName);

#endif
