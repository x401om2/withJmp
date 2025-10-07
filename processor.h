#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include "stack.h"

int processingFromFile(const char* filename, processor_t* processor);

const char* getRegisterName(int regIndex);

void executeProcessor(processor_t* processor);
void processCommand(processor_t* processor, int command);

#endif
