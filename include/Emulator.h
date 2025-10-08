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
#include <random>
#include <stack>
#include <ncurses.h>
//#include "Display.h"
#include "Registers.h"
#include </opt/homebrew/include/SDL2/SDL.h>
#include "Instructions.h"
#include "util.h"



// Utils: struct/class definitions, utility functions

// Constants
const unsigned int MEMORY_SIZE = 4096;  // 4KB Memory Size
const unsigned int CLOCK_SPEED = 700;   // 700 Hz
const uint16_t BASE_ADDRESS    = 0x200; // where the program should start in memory
const uint16_t FONT_ADDRESS    = 0x050;
const int      FONT_SIZE       = 5;     // Size of a single character sprite in memory
//const int      STACK_SIZE      = 10;    // How many stack frames


// Display shit
const uint8_t DISPLAY_HEIGHT = 32;
const uint8_t DISPLAY_WIDTH  = 64;
const uint8_t SPACING        = 20;

const uint16_t ABSOLUTE_HEIGHT = DISPLAY_HEIGHT * SPACING;
const uint16_t ABSOLUTE_WIDTH  = DISPLAY_WIDTH  * SPACING;

// Timer shit
const unsigned int DELAY  = 60;    // 60 Hz
auto lastTimerUpdate = std::chrono::high_resolution_clock::now();


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
    std::stack<unsigned int> stack;   // used for pushing the current program counter to
    Registers registers;
    bool debugFlag;
    uint8_t delayTimer;
    uint8_t soundTimer;

    
    // Methods
    int LoadROM(std::string& filePath);
    int DisplayAddressSpace(unsigned int maxAddress);
    void DisplayRegisters();
    void SetGeneralRegisterValue(int registerNumber, uint8_t value);
    uint8_t GetGeneralRegisterValue(int registerNumber);
    int Run();
    
    
    //Display attributes;
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    // Display Methods
    void WriteBlocks();
    
    
    Emulator(bool debug, unsigned int size = MEMORY_SIZE) {
        std::cout << "Hi, from the emulator" << std::endl;
        
        addressSpaceSize = size;
        
        debugFlag = debug;
        
        delayTimer = 0xff;
        soundTimer = 0xff;
        
        //initscr();
        
        //nodelay(stdscr, TRUE);
        
        // Initialize the address space
        addressSpace = new int[addressSpaceSize];
        
        // Copy the font into memory
        std::ifstream inputFile("./font/font.txt"); // Replace with your file name
        if (!inputFile.is_open()) {
            std::cerr << "Error opening font file!" << std::endl;
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
        
        
        for (size_t i = 0; i < buffer.size(); i ++) {
            // starting at address 0x200 -> start to copy each byte to memory
            addressSpace[FONT_ADDRESS + i] = buffer[i];;
        }
        
        
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
    

    
    
    int running = 1;
    SDL_Event event;
    
    INSTRUCTION currentInstr;
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
                
                switch(restOfInstr) {
                    case 0x0E0: {
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
                    case 0x0EE: {
                        // Pop the PC off the stack and update PC
                        registers.PC = static_cast<uint16_t>(stack.top());
                        stack.pop();
                    }
                }
                break;
            }
                
            case 0x1: {
                // handle opcode 1; Jump to Address
                registers.PC = restOfInstr;
                break;
            }
            case 0x2: {
                // handle opcode 2; Jump to Address
                registers.PC = restOfInstr;
                
                // push the PC to the stack
                stack.push(static_cast<unsigned int>(registers.PC));
                break;
            }
            case 0x3: {
                // 3XNN: will skip one instruction if VX == NN
                // Get the value of VX
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                // Get Value of NN
                int NN = restOfInstr & 0xff;
                
                // Check if equal to NN
                if (xValue == NN) {
                    // Update program counter
                    registers.PC += 2;
                }
                
                break;
                
            }
            case 0x4: {
                // 4XNN: will skip one instruction if VX != NN
                // Get the value of VX
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                int NN = restOfInstr & 0xff;
                
                if (xValue != NN) {
                    // Update program counter
                    registers.PC += 2;
                }
                break;
            }
            case 0x5: {
                // 5XY0: will skip one instruction if VX == VY
                // Get the value of VX
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                // Get the value of VY
                int yReg = (restOfInstr >> 4) & 0xf;
                uint8_t yValue = GetGeneralRegisterValue(yReg);
                // Check if equal
                if (xValue == yValue) {
                    registers.PC += 2;
                }
                
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
            case 0x8: {
                // handle opcode 8; arithmetic instructions
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);

                int yReg = (restOfInstr >> 4) & 0xf;
                uint8_t yValue = GetGeneralRegisterValue(yReg);

                int N = (restOfInstr & 0xf);
                
                // 8XY0: VX is set to the value of VY
                switch(N) {
                    case 1: {
                        // 8XY1: Logical OR -> VX is set to the bitwise OR of VX, VY; VY is unaffected
                        uint8_t result = xValue | yValue;
                        SetGeneralRegisterValue(xReg, result);
                        break;
                    }
                    case 2: {
                        // 8XY2: Logical    -> AND VX is set to the bitwise AND of VX, VY; VY is unaffected
                        uint8_t result = xValue & yValue;
                        SetGeneralRegisterValue(xReg, result);
                        break;
                    }
                    case 3: {
                        // 8XY3: Logical    -> XOR VX is set to the bitwise XOR of VX, VY; VY is unaffected
                        uint8_t result = xValue ^ yValue;
                        SetGeneralRegisterValue(xReg, result);
                        break;
                    }
                    case 4: {
                        // 8XY4: Add        -> VX is set to the sum of VX and VY; VY is unaffected, if the overflow is 1 set VF to 1 otherwise set to 0
                        int sum = 0;
                        int carry = 0;
                            
                        AddBinaryNumbers(xValue, yValue, &sum, &carry);
                        
                        SetGeneralRegisterValue(xReg, static_cast<uint8_t>(sum));
                        registers.VF = static_cast<uint8_t>(carry);
                        
                        break;
                    }
                    case 5: {
                        // 8XY5: Subtract   -> VX is set to VX - VY; VY is unaffected
                        
                        yValue = (~yValue + 1);
                        
                        int sum = 0;
                        int carry = 0;
                        
                        AddBinaryNumbers(xValue, yValue, &sum, &carry);
                        
                        SetGeneralRegisterValue(xReg, static_cast<uint8_t>(sum));
                        registers.VF = static_cast<uint8_t>(carry);
                        
                        break;
                    }
                    case 6: {
                        // 8XY6: Bit Shift  -> Set VX to value of VY, shift value of VX one bit to right; set VF to the bit that was truncated out
                        uint8_t carry = yValue & 0x1;
                        uint8_t result = yValue >> 1;
                        
                        SetGeneralRegisterValue(xReg, result);
                        registers.VF = carry;
                        
                        break;
                    }
                    case 7: {
                        // 8XY7: Subtract   -> VX is set to VY - VX; VY is unaffected
                        xValue = (~xValue + 1);
                        
                        int sum = 0;
                        int carry = 0;
                        
                        AddBinaryNumbers(xValue, yValue, &sum, &carry);
                        
                        SetGeneralRegisterValue(xReg, static_cast<uint8_t>(sum));
                        registers.VF = static_cast<uint8_t>(carry);
                        
                        break;
                    }
                    case 8: {
                        // 8XY8: Bit Shift  -> Set VX to value of VY, shift value of VX one bit to left; set VF to the bit that was truncated out
                        uint8_t carry = (xValue & 0x80) >> 7; //10000000
                        uint8_t result = static_cast<uint8_t>(xValue << 1);
                        
                        SetGeneralRegisterValue(xReg, result);
                        registers.VF = carry;
                        break;
                    }
                        
                }
  
                
                break;
            }
            case 0x9: {
                // 9XY0: will skip one instruction if VX != VY
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                // Get the value of VY
                int yReg = (restOfInstr >> 4) & 0xf;
                uint8_t yValue = GetGeneralRegisterValue(yReg);
                // Check if not equal
                if (xValue != yValue) {
                    registers.PC += 2;
                }
                
                break;
            }
            case 0xA: {
                // handle opcode A; Set register I to a value
                registers.I = restOfInstr;
                break;
            }
            case 0xB: {
                // BXNN: jumps with offset, NN + V0 value
                
                // Get NN
                uint16_t NNN = restOfInstr & 0xfff;
                
                registers.PC = NNN + registers.V0;
                break;
                
            }
            case 0xC: {
                // CXNN: Generates a random number, ANDs it with NN and then puts the value in VX
                int xReg = (restOfInstr >> 8);
                
                int NN = restOfInstr & 0xff;
                
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(0, 255); // 8 bit number

                int randomValue = distrib(gen);
                
                //std::cout << "Random Value: " << randomValue << std::endl;
                
                SetGeneralRegisterValue(xReg, static_cast<uint8_t>(randomValue & NN));
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
                        curBit = mask & nextByte;
                        
                        if (curBit && displayGrid[xValue][yValue]) {
                            displayGrid[xValue][yValue] = 0;
                            SetGeneralRegisterValue(0xf, 1);
                        } else if (curBit && ~displayGrid[xValue][yValue]) {
                            displayGrid[xValue][yValue] = 1;
                        }
                        
                        xValue = ((xValue += 1));

                        mask = (mask >> 1);
                        idx += 1;
                    }
                    yValue = ((yValue += 1));

                }
                
                break;
            }
            case 0xE: {
            
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                uint8_t fCode = currentInstr & 0xff;
                
                switch(fCode) {
                    case 0x9e: {
                        // EX9E: skips one instruction if the key pressed matches value in VX
                        
                        int ch = getch();
                        
                        std::cout << ch << std::endl;
                        
                        if (ch == static_cast<int>(xValue)) { // Check if a key has been pressed
                            
                            std::cout << "You pressed the value of VX." << std::endl;
                            
                        }
                        
                        
                        break;
                    }
                    case 0xa1: {
                        // EXA1: skips one instruction if the key pressed does not match value in VX
                        
                        
                        break;
                    }
                }
                
                
                
                break;
            }
            case 0xF: {
                
                
                int xReg = (restOfInstr >> 8);
                uint8_t xValue = GetGeneralRegisterValue(xReg);
                
                uint8_t fCode = currentInstr & 0xff;
                
                switch(fCode) {
                    case 0x1e: {
                        registers.I = registers.I + xValue;
                        break;
                    }
                    case 0x29: {
                        // FX29: store the font address of the hexadecimal value in VX
                        
                        // Calculate the address that the font for char VX is
                        uint16_t address = FONT_ADDRESS + (xValue * FONT_SIZE);
                        
                        registers.I = address;
                        break;
                    }
                    case 0x07: {
                        // FX07 sets VX to the current value of the delay timer
                        SetGeneralRegisterValue(xReg, delayTimer);
                        break;
                    }
                    case 0x15: {
                        // FX15 sets the delay timer to the value in VX
                        delayTimer = xValue;
                        break;
                    }
                    case 0x18: {
                        // FX18 sets the sound timer to the value in VX
                        soundTimer = xValue;
                        break;
                    }
                }
                
                break;
                
            }
            default: {
                std::cout << "Instruction Not Implemented. Try Again Later." << std::endl;
            }
        }
        
        // Update the display
        WriteBlocks();
        SDL_RenderPresent(renderer);
         
        // handle timers
        auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> elapsed = now - lastTimerUpdate;
            if (elapsed.count() >= 1.0f / 60.0f) {
                if (delayTimer > 0) delayTimer--;
                if (soundTimer > 0) {
                    soundTimer--;
                    if (soundTimer > 0) {
                        // Beep on
                    } else {
                        // Beep off
                    }
                }
                lastTimerUpdate = now;
            }
        
        
        // wait
        std::this_thread::sleep_for(std::chrono::seconds(1 / CLOCK_SPEED));
        if (debugFlag) {
            // Display the values of the timers
            std::cout << "Delay Timer: " << static_cast<int>(delayTimer) << std::endl;
            std::cout << "Sound Timer: " << static_cast<int>(soundTimer) << std::endl;
            // Display the registers
            DisplayRegisters();
            
            std::cout << "Enter for next loop iteration" << std::endl;
            std::cin >> running;
        }

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
    
    
    for (size_t i = 0; i < buffer.size(); i ++) {
        // starting at address 0x200 -> start to copy each byte to memory
        addressSpace[BASE_ADDRESS + i] = buffer[i];;
    }
    
    // initialize the program counter
    registers.PC = BASE_ADDRESS;
    
    
    return 0;
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
