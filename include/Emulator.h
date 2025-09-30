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
#include <bitset>
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
        std::cout << "V0: " << std::hex << static_cast<int>(V0) << std::endl; // Have to cast these to ints so they are visible on screen
        std::cout << "V1: " << std::hex << static_cast<int>(V1) << std::endl;
        std::cout << "V2: " << std::hex << static_cast<int>(V2) << std::endl;
        std::cout << "V3: " << std::hex << static_cast<int>(V3) << std::endl;
        std::cout << "V4: " << std::hex << static_cast<int>(V4) << std::endl;
        std::cout << "V5: " << std::hex << static_cast<int>(V5) << std::endl;
        std::cout << "V6: " << std::hex << static_cast<int>(V6) << std::endl;
        std::cout << "V7: " << std::hex << static_cast<int>(V7) << std::endl;
        std::cout << "V8: " << std::hex << static_cast<int>(V8) << std::endl;
        std::cout << "V9: " << std::hex << static_cast<int>(V9) << std::endl;
        std::cout << "VA: " << std::hex << static_cast<int>(VA) << std::endl;
        std::cout << "VB: " << std::hex << static_cast<int>(VB) << std::endl;
        std::cout << "VC: " << std::hex << static_cast<int>(VC) << std::endl;
        std::cout << "VD: " << std::hex << static_cast<int>(VD) << std::endl;
        std::cout << "VE: " << std::hex << static_cast<int>(VE) << std::endl;
        std::cout << "VF: " << std::hex << static_cast<int>(VF) << std::endl;
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
    void DisplayRegisters();
    void SetGeneralRegisterValue(int registerNumber, uint8_t value);
    uint8_t GetGeneralRegisterValue(int registerNumber);
    int Run();
    bool debugFlag;
    
    
    //Display attributes;
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    // Display Methods
    void WriteBlocks();
    
    // Instruction Methods
    //int ClearScreen();
    
    
    Emulator(bool debug, unsigned int size = MEMORY_SIZE) {
        std::cout << "Hi, from the emulator" << std::endl;
        
        addressSpaceSize = size;
        
        debugFlag = debug;
        
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
    
    //int test_idx = 0;
    
    // * Think about exit conditions for this loop
    INSTRUCTION currentInstr;
    while (running) {
        /*
        // For testing
        if (test_idx > 20) {
            running = 0;
        }
         */
         
         
         
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
        std::cout << "Fetch Instruction" << std::endl;
        currentInstr = static_cast<INSTRUCTION>(addressSpace[registers.PC] << 8 | addressSpace[registers.PC + 1]);
        std::cout << "Current Instruction: " << std::hex << currentInstr << std::endl;
        
        // Update the program counter
        registers.PC += 2;
        
        // Decode the instruction: Determine the kind of instruction
        // To get the opcode -> shift 12 bits
        OPCODE opCode = currentInstr >> 12;
        std::cout << "Current Instruction Opcode: " << std::hex << opCode << std::endl << std::endl;
        
        // Get the last 12 bits of the currentInstr with bit mask
        uint16_t restOfInstr = currentInstr & 0xfff;
        
        switch(opCode) {
            // Starting off with these 6 basic instructions (clear screen, jump, set register, add value to register, set register I, draw to display)
            case 0x0: {
                // handle opcode 0; Clear Screen
                for (int i = 0; i < DISPLAY_WIDTH; i ++) {
                    for (int j = 0; j < DISPLAY_HEIGHT; j ++) {
                        displayGrid[i][j] = 0;
                        //displayGrid[DISPLAY_WIDTH - 1][DISPLAY_HEIGHT - 1] = 1;
                    }
                }
                WriteBlocks();
                break;
            }
                
            case 0x1: {
                // handle opcode 1; Jump to Address
                registers.PC = restOfInstr;
                break;
            }
            case 0x6: {
                // handle opcode 6; Set register to Value
                int registerNumber = restOfInstr >> 8; // Obtain the register from the restOfInstr
                
                uint8_t value = static_cast<uint8_t>(restOfInstr & 0xff);
        
                // Set the register to value
                
                SetGeneralRegisterValue(registerNumber, value);
                break;
            }
            case 0x7: {
                // handle opcode 7; Add value to a register
                
                int registerNumber = restOfInstr >> 8; // Obtain the register from the restOfInstr
                // Obtain the register value and add it to the operand -> set value for the register
                uint8_t value = static_cast<uint8_t>(GetGeneralRegisterValue(registerNumber) + (restOfInstr & 0xff));
                SetGeneralRegisterValue(registerNumber, value);
                
                break;
            }
            case 0xA: {
                // handle opcode A; Set register I to a value
                registers.I = restOfInstr;
                break;
            }
            case 0xD: {
                // handle opcode D; Display to the screen
                // DXYN, N pixels tall, starting at coordinate XY, draw the sprite at address stored in I register
                // op   VX   VY   N
                // 0000 0001 0002 000f
                // Obtain the x coordinate: Value in the VX register % DISPLAY_WIDTH
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg) % DISPLAY_WIDTH;
                std::cout << "X Value: " << static_cast<int>(xValue) << std::endl;
                // Obtain the y coordinate: Value in the VY register % DISPLAY_HEIGHT
                int yReg = (restOfInstr >> 4) & 0xf;
                uint8_t yValue = GetGeneralRegisterValue(yReg) % DISPLAY_HEIGHT;
                std::cout << "Y Value: " << static_cast<int>(yValue) << std::endl;
                // Obtain N
                int N = (restOfInstr & 0xf);
                std::cout << "N: " << N << std::endl;
                // Set VF to 0
                SetGeneralRegisterValue(0xf, 0);
                
                int nextByte;
                
                for (int n = 0; n < N; n ++) {
                    // Get nth byte of sprite data (I + n): Get address in I register -> get data at that address + n
                    nextByte = addressSpace[(registers.I + n)];
                    
                    std::bitset<8> x(static_cast<unsigned long long>(nextByte));
                    std::cout << "Byte: " << x << std::endl;

                    xValue = GetGeneralRegisterValue(xReg) % DISPLAY_WIDTH;
                    
                    int idx = 0;
                    bool curBit;
                    int mask = 0x80;
                    while (idx < 8) {
                        // if we reach the end of the screen; break this loop
                        if (xValue > DISPLAY_WIDTH) {
                            break;
                        }
                        
                        // get the next bit
                        //curBit = nextByte & 0x1;
                        curBit = mask & nextByte;
                        
                        if (curBit && displayGrid[xValue][yValue]) {
                            //std::cout << "X, Y: " << xValue << yValue << std::endl;
                            displayGrid[xValue][yValue] = 0;
                            SetGeneralRegisterValue(0xf, 1);
                        } else if (curBit && ~displayGrid[xValue][yValue]) {
                            displayGrid[xValue][yValue] = 1;
                        }
                        
                        
                        xValue = ((xValue += 1));
                        
                        //nextByte = nextByte >> 1;
                        mask = (mask >> 1);
                        idx += 1;
                    }
                    yValue = ((yValue += 1));
                    
                    
                
                }
                
                
                break;
            }
            default: {
                std::cout << "Instruction Not Implemented. Try Again Later." << std::endl;
            }
        }
        WriteBlocks();
        // Update the display
        //std::cout << "Displaying" << std::endl;
        SDL_RenderPresent(renderer);
         
        // wait for 1/60 seconds
        std::this_thread::sleep_for(std::chrono::seconds(1 / FREQUENCY));
        if (debugFlag) {
            std::cout << "Enter for next loop iteration" << std::endl;
            std::cin >> running;
        }
        
        DisplayRegisters();
        
        //test_idx += 1;
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
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
    registers.PC = BASE_ADDRESS;
    
    
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



void Emulator::DisplayRegisters() {

    // Displays all of the registers and their values
    
    registers.display();

}


void Emulator::SetGeneralRegisterValue(int registerNumber, uint8_t value) {
    
    switch(registerNumber) {
        case 0: {
            registers.V0 = value;
            break;
        }
        case 1: {
            registers.V1 = value;
            break;
        }
        case 2: {
            registers.V2 = value;
            break;
        }
        case 3: {
            registers.V3 = value;
            break;
        }
        case 4: {
            registers.V4 = value;
            break;
        }
        case 5: {
            registers.V5 = value;
            break;
        }
        case 6: {
            registers.V6 = value;
            break;
        }
        case 7: {
            registers.V7 = value;
            break;
        }
        case 8: {
            registers.V8 = value;
            break;
        }
        case 9: {
            registers.V9 = value;
            break;
        }
        case 0xA: {
            registers.VA = value;
            break;
        }
        case 0xB: {
            registers.VB = value;
            break;
        }
        case 0xC: {
            registers.VC = value;
            break;
        }
        case 0xD: {
            registers.VD = value;
            break;
        }
        case 0xE: {
            registers.VE = value;
            break;
        }
        case 0xF: {
            registers.VF = value;
            break;
        }
        default:
            std::cout << "Unrecognized Register: Failed to update." << std::endl;
    }
    
    
}

uint8_t Emulator::GetGeneralRegisterValue(int registerNumber) {
    
    switch(registerNumber) {
    case 0:
            return registers.V0;
    case 1:
            return registers.V1;
    case 2:
            return registers.V2;
    case 3:
            return registers.V3;
    case 4:
            return registers.V4;
    case 5:
            return registers.V5;
    case 6:
            return registers.V6;
    case 7:
            return registers.V7;
    case 8:
            return registers.V8;
    case 9:
            return registers.V9;
    case 0xA:
            return registers.VA;
    case 0xB:
            return registers.VB;
    case 0xC:
            return registers.VC;
    case 0xD:
            return registers.VD;
    case 0xE:
            return registers.VE;
    case 0xF:
            return registers.VF;
    }
    return 0;
    
    
}




#endif // !EMULATOR_H
