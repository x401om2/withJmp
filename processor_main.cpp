#include <stdio.h>
#include <math.h>
#include <stack.h>
#include <processorOperations.h>
#include "processor.h"

int main()
{
    processor_t cpu;

    processorCtor(&cpu);
    // processorDump(&cpu, "Создан");

    processingFromFile("processor.bin", &cpu);

    // processorDump(&cpu, "Завершили работу");

    return 0;
}
