#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include </opt/homebrew/include/SDL2/SDL.h>


const uint8_t DISPLAY_HEIGHT = 32;
const uint8_t DISPLAY_WIDTH  = 64;

const uint16_t ABSOLUTE_HEIGHT = 640;
const uint16_t ABSOLUTE_WIDTH = 1300;

const uint8_t SPACING = 20;



class Display{
public:
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    //uint8_t spacing = SPACING;
    
    Display() {
        // Constructor for the Display
        
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            // Handle Error
            std::cout << "Error while Initializing SDL. Try Again..." << std::endl;
            return;
        }
        
        // Create the window
        window = SDL_CreateWindow("Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ABSOLUTE_WIDTH, ABSOLUTE_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            // Handle Error
            std::cout << "Error while Creating Window. Try Again..." << std::endl;
            return;
        }
        std::cout << "Successfully Created Window!" << std::endl;

        
        // Creating Renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            // Handle Error
            std::cout << "Error while Creating Renderer. Try Again..." << std::endl;
            return;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        std::cout << "Successfully Created Renderer!" << std::endl;
        
        
    }
    
    
    void DrawGrid();
    
    
};


void Display::DrawGrid() {
 
    SDL_Event e;
    bool quit = false;
    
    while (!quit) {
        
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT){
            quit = true;
        }
        if (e.type == SDL_KEYDOWN){
            quit = true;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN){
            quit = true;
        }
        
        // Draw a series of horizontal and vertical lines to create a grid
        std::cout << "Updating Renderer!" << std::endl;
        for (int x = 0; x < ABSOLUTE_WIDTH; x += SPACING) {
            SDL_RenderDrawLine(renderer, x, 0, x, ABSOLUTE_HEIGHT);
        }
        
        for (int y = 0; y < ABSOLUTE_HEIGHT; y += SPACING) {
            SDL_RenderDrawLine(renderer, 0, y, ABSOLUTE_WIDTH, y);
        }
        
        std::cout << "Displaying Lines!" << std::endl;
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    
}




#endif // !DISPLAY_H
