#ifndef EMULATOR_H
#define EMULATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <fstream>
#include <chrono>
#include <thread>
#include "Display.h"
#include "Instructions.h"


// Utils: struct/class definitions, utility functions

// Constants
const unsigned int MEMORY_SIZE = 4096; // 4KB Memory Size
const unsigned int FREQUENCY   = 60;   // 60 Hz



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



/* Address Space Layout (4096 bytes)

 12 bit indexing
 
 * Historically, the interpreter was located in memory from address 0x000 to 0x1FF.
 * In order to preserve compatibility with CHIP 8 programs found online, I will
 * be leaving this part of the addres space empty.
 
 * ROM should be loaded into 0x200
 
 * EACH INSTRUCTION IS TWO BYTES: FETCH INSTRUCTIONS IN TWO BYTE INCREMENTS
 
 Address Space
 
 
 
 
 
 
*/



// Class to define the emulator
class Emulator {
private:
    // Private Attributes
    int* addressSpace;

public:
    // Public Attributes
    unsigned int addressSpaceSize;
    // Methods
    int LoadROM(std::string filePath);
    int Run();
    Display display;
    
    Emulator(unsigned int size = MEMORY_SIZE) {
        std::cout << "Hi, from the emulator" << std::endl;
        
        addressSpaceSize = size;
        
        // Initialize the registers
        [[maybe_unused]]Registers registers;
        
        
        // Initialize the address space
        addressSpace = new int[addressSpaceSize];
        
    }
    
    
    
};



int Emulator::Run() {
    // returns 1 on error; 0 on success
    
    
    std::cout << "Running the Emulator" << std::endl;
    
    // Starting at where the PC counter is pointing to, begin to fetch -> decode -> execute instructions
    
    // * LOOP NEEDS TO RUN AT 60Hz (60 cycles a second / 60 iterations a second)
    
    // * Think about exit conditions for this loop
    //INSTRUCTION currentInstr;
    while (true) {
        
        // Fetch next instruction
        
        
        // Decode the instruction
        
        
        // Execute the instruction
        
        
        // Update the program counter
        
        
        // wait for 1/60 seconds
        std::this_thread::sleep_for(std::chrono::seconds(1 / FREQUENCY));
    }
    
    return 0;
}





int Emulator::LoadROM(std::string filePath) {
    // returns 0 on success, 1 on failure
    std::string message = "Loading Program " + filePath + " to ROM!";
    
    std::cout << message << std::endl;
    
    std::fstream inputFile(filePath);
    
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file: " << filePath << std::endl;
        return 1;
    }
    
    
    /*
    // For testing, let's display the file contents
    std::string line;
    int c = 0;
    while (std::getline(inputFile, line)) {
        std::cout << "Read from file: " << line << std::endl;
        c += 1;
    }
    
    std::cout << "Total Number of Lines: " << c << std::endl;
    */
    
    // Copy the program contents to addressSpace at 0x200
    
    
    
    return 0;
}

#endif // !EMULATOR_H
