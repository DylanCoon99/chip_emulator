#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

// Instruction typedef
typedef uint16_t INSTRUCTION;


// Instruction Execute Functionality
int ExecuteInstruction(INSTRUCTION instr) {
    
    std::cout << "Received Instruction: " << instr << std::endl;
    
    return 0;
}


#endif // !INSTRUCTIONS_H
