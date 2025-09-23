#ifndef EMULATOR_H
#define EMULATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

// Utils: struct/class definitions, utility functions

// Constants
const unsigned int MEMORY_SIZE = 4096; // 4KB Memory Size



// Registers
struct Registers {
    uint16_t PC; // Program Counter (PC): 12 bits, points to current instruction that in memory
    uint16_t I;  // One 16 bit index register called "I": used to point to locations in memory
    uint8_t V1;  // 16 8-bit (1 byte) general purpose registers numbered 0x0 - 0xF (V0 - VF)
    uint8_t V2;
    uint8_t V3;
    uint8_t V4;
    uint8_t V5;
    uint8_t V6;
    uint8_t V7;
    uint8_t V8;
    uint8_t V9;
    uint8_t VA;
    uint8_t VB;
    uint8_t VC;
    uint8_t VD;
    uint8_t VE;
    uint8_t VF;
};


// Class to define the emulator
class Emulator {
private:
    // Private Attributes
    int* addressSpace;

public:
    // Public Attributes
    unsigned int addressSpaceSize;
    // Methods
    int LoadROM();
    
    Emulator(unsigned int size = MEMORY_SIZE) {
        std::cout << "Hi, from the emulator" << std::endl;
        
        addressSpaceSize = size;
        
        // Initialize the registers
        [[maybe_unused]]Registers registers;
        
        
        // Initialize the address space
        addressSpace = new int[addressSpaceSize];
        
    }
    
    
    
};


int Emulator::LoadROM() {
    
    std::cout << "Loading Program to ROM!" << std::endl;
    
    return 0;
}

#endif // !EMULATOR_H
