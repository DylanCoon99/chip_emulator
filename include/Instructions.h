#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

// Instruction typedef
typedef uint16_t INSTRUCTION;

// Opcode
typedef int OPCODE;


// Instruction Executions
int ClearScreen() {

    return 0;
}



// Instruction Execute Functionality
int ExecuteInstruction(INSTRUCTION instr) {
    
    std::cout << "Received Instruction: " << instr << std::endl;
    
    return 0;
}


#endif // !INSTRUCTIONS_H
