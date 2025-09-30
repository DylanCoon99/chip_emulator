#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include </opt/homebrew/include/SDL2/SDL.h>
#include <chrono>
#include <thread>
#include <random>


void Emulator::WriteBlocks() {
    
    // writes block to white (val = 1) or black (val = 0)
    int x_start_coord;
    int y_start_coord;
    
    int val;

    for (int i = 0; i < DISPLAY_WIDTH; i ++ ) {
        for (int j = 0; j < DISPLAY_HEIGHT; j ++ ) {
            
            // Obtain the value
            val = displayGrid[i][j];
            
            // Set coordinates
            x_start_coord = i * SPACING;
            y_start_coord = j * SPACING;
            
            if (val == 1) {
                // Set color to white
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                // Set color to black
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            
            // Draw a box on these coordinates
            SDL_Rect block = {x_start_coord, y_start_coord, SPACING, SPACING};
            
            SDL_RenderFillRect(renderer, &block);
        }
    }
    
}


void Emulator::DisplayRegisters() {
    // Displays all of the registers and their values
    
    registers.display();

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





#endif // !DISPLAY_H
