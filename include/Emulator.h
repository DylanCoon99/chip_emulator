#ifndef EMULATOR_H
#define EMULATOR_H


#include <stdio.h>
#include <stdlib.h>

// Utils: struct/class definitions, utility functions

// Constants
const unsigned int MEMORY_SIZE = 4096; // 4KB memory size

// class to define the emulator
class Emulator {
public:
    Emulator() {
        std::cout << "Hi, from the emulator" << std::endl;
    }
    
    int addressSpaceSize;
    int addressSpace[];
    
};

#endif // !EMULATOR_H
