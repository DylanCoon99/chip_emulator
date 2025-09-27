#ifndef EMULATOR_H
#define EMULATOR_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
//#include "Display.h"
#include </opt/homebrew/include/SDL2/SDL.h>
#include "Instructions.h"
#include "util.h"


// Utils: struct/class definitions, utility functions

// Constants
const unsigned int MEMORY_SIZE = 4096; // 4KB Memory Size
const unsigned int FREQUENCY   = 700;   // 700 Hz
const uint16_t BASE_ADDRESS = 0x200; // where the program should start in memory


// Display shit
const uint8_t DISPLAY_HEIGHT = 32;
const uint8_t DISPLAY_WIDTH  = 64;
const uint8_t SPACING        = 20;

const uint16_t ABSOLUTE_HEIGHT = DISPLAY_HEIGHT * SPACING;
const uint16_t ABSOLUTE_WIDTH  = DISPLAY_WIDTH  * SPACING;



// Registers
struct Registers {
    uint16_t PC; // Program Counter (PC): 12 bits, points to current instruction that in memory
    uint16_t I;  // One 16 bit index register called "I": used to point to locations in memory
    uint8_t V0;  // 16 8-bit (1 byte) general purpose registers numbered 0x0 - 0xF (V0 - VF)
    uint8_t V1;
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

    
    void display() {
        std::cout << "PC: " << std::hex << PC << std::endl;
        std::cout << "I:  " << std::hex << I << std::endl;
        std::cout << "V0: " << std::hex << V0 << std::endl;
        std::cout << "V1: " << std::hex << V1 << std::endl;
        std::cout << "V2: " << std::hex << V2 << std::endl;
        std::cout << "V3: " << std::hex << V3 << std::endl;
        std::cout << "V4: " << std::hex << V4 << std::endl;
        std::cout << "V5: " << std::hex << V5 << std::endl;
        std::cout << "V6: " << std::hex << V6 << std::endl;
        std::cout << "V7: " << std::hex << V7 << std::endl;
        std::cout << "V8: " << std::hex << V8 << std::endl;
        std::cout << "V9: " << std::hex << V9 << std::endl;
        std::cout << "VA: " << std::hex << VA << std::endl;
        std::cout << "VB: " << std::hex << VB << std::endl;
        std::cout << "VC: " << std::hex << VC << std::endl;
        std::cout << "VD: " << std::hex << VD << std::endl;
        std::cout << "VE: " << std::hex << VE << std::endl;
        std::cout << "VF: " << std::hex << VF << std::endl;
    }
};



/* Address Space Layout (4096 bytes)

 12 bit indexing
 
 * Historically, the interpreter was located in memory from address 0x000 to 0x1FF.
 * In order to preserve compatibility with CHIP 8 programs found online, I will
 * be leaving this part of the address space empty.
 
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
    Registers registers;
    // Methods
    int LoadROM(std::string& filePath);
    int DisplayAddressSpace(unsigned int maxAddress);
    int DisplayRegisters();
    int Run();
    
    
    //Display display;
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    
    Emulator(unsigned int size = MEMORY_SIZE) {
        std::cout << "Hi, from the emulator" << std::endl;
        
        addressSpaceSize = size;
        
        
        // Initialize the address space
        addressSpace = new int[addressSpaceSize];
        
    }
    
    
    
};



int Emulator::Run() {
    
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ABSOLUTE_WIDTH, ABSOLUTE_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (window == NULL) {
        // Handle Error
        std::cerr << "Error while Creating Window. Try Again..." << std::endl;
        return 1;
    }
    std::cout << "Successfully Created Window!" << std::endl;
    
    // Creating Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        // Handle Error
        std::cerr << "Error while Creating Renderer. Try Again..." << std::endl;
        return 1;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::cout << "Successfully Created Renderer!" << std::endl;
    
    
    
    
    //std::cout << "Running the Emulator" << std::endl;
    
    // Starting at where the PC counter is pointing to, begin to fetch -> decode -> execute instructions
    
    // * LOOP NEEDS TO RUN AT 60Hz (60 cycles a second / 60 iterations a second)
    
    int running = 1;
    SDL_Event event;
    
    
    // * Think about exit conditions for this loop
    //INSTRUCTION currentInstr;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            // Handle keyboard input, mouse events, etc.
            if (event.type == SDL_QUIT){
                running = 0;
            }
            if (event.type == SDL_KEYDOWN){
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                running = 0;
            }
        }
        
        // Fetch next instruction
        std::cout << "Simulating Fetch Instruction" << std::endl;
        
        // Decode the instruction
        std::cout << "Simulating Decode Instruction" << std::endl;
        
        // Execute the instruction
        std::cout << "Simulating Execute Instruction" << std::endl;
        
        // Update the program counter
        
        
        // wait for 1/60 seconds
        std::this_thread::sleep_for(std::chrono::seconds(1 / FREQUENCY));
        
        
    }
    
    return 0;
}





int Emulator::LoadROM(std::string& filePath) {
    // returns 0 on success, 1 on failure
    std::string message = "Loading Program " + filePath + " to ROM!";
    std::cout << message << std::endl;
    
    std::ifstream inputFile(filePath); // Replace with your file name
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<int> buffer;
    char char1, char2;

    while (inputFile >> char1 >> char2) {
        int val1 = hexCharToInt(char1);
        int val2 = hexCharToInt(char2);

        if (val1 != -1 && val2 != -1) {
            int byte = (val1 << 4) | val2;
            buffer.push_back(byte);
            //std::cout << "Hex Char: " << std::hex << byte << std::endl;
        } else {
            std::cerr << "Invalid hex character encountered: " << char1 << char2 << std::endl;
            // Handle error or skip invalid characters
            continue;
        }
    }

    inputFile.close();
    
    // Copy the program contents to addressSpace at 0x200
    //std::cout << buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
    
    
    
    for (size_t i = 0; i < buffer.size(); i ++) {
        //std::cout << buffer[i] << buffer[i + 1] << std::endl;
        // starting at address 0x200 -> start to copy each byte to memory
        addressSpace[BASE_ADDRESS + i] = buffer[i];;
    }
    
    // initialize the program counter (other necessary registers?)
    registers.PC = 0x200;
    
    
    return 0;
}



int Emulator::DisplayAddressSpace(unsigned int maxAddress) {
    // returns 0 on success, 1 on failure

    // Displays the address space for the emulator
    for (unsigned int i = 0; i < maxAddress; i ++) {
        // Displays all of the bytes
        std::cout << "Address: " << std::hex << i << " " << addressSpace[i] << std::endl;
        
    }


    return 0;
}



int Emulator::DisplayRegisters() {
    // returns 0 on success, 1 on failure

    // Displays all of the registers and their values
    
    registers.display();


    return 0;
}




#endif // !EMULATOR_H
