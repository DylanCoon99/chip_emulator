#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include </opt/homebrew/include/SDL2/SDL.h>


const uint8_t DISPLAY_HEIGHT = 32;
const uint8_t DISPLAY_WIDTH  = 64;



class Display{
public:
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    void DrawGrid();
    
    
};


void Display::DrawGrid() {
 
    // Draw a series of horizontal and vertical lines to create a grid
    
    
}




#endif // !DISPLAY_H
