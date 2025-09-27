#ifndef DISPLAY_H
#define DISPLAY_H


#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include </opt/homebrew/include/SDL2/SDL.h>
#include <chrono>
#include <thread>
#include <random>



/*

class Display{
public:
    int displayGrid[DISPLAY_WIDTH][DISPLAY_HEIGHT];
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
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
            std::cerr << "Error while Creating Window. Try Again..." << std::endl;
            return;
        }
        std::cout << "Successfully Created Window!" << std::endl;

        
        // Creating Renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            // Handle Error
            std::cerr << "Error while Creating Renderer. Try Again..." << std::endl;
            return;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        std::cout << "Successfully Created Renderer!" << std::endl;
        
        
    }
    
    void InitDisplay();
    void DrawGrid();
    
private:
    void WriteBlock(int x, int y, int val);

    
    
};


void Display::InitDisplay() {
    // inits the displayGrid with some sample data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    
    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        for (int j = 0; j < DISPLAY_HEIGHT; j++) {
            int randomNumber = distrib(gen);
            //std::cout << "Random Number: " << randomNumber << std::endl;
            displayGrid[i][j] = randomNumber;
        }
    }
    
}





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
        //std::cout << "Updating Renderer!" << std::endl;
        for (int x = 0; x < ABSOLUTE_WIDTH; x += SPACING) {
            SDL_RenderDrawLine(renderer, x, 0, x, ABSOLUTE_HEIGHT);
        }
        
        for (int y = 0; y < ABSOLUTE_HEIGHT; y += SPACING) {
            SDL_RenderDrawLine(renderer, 0, y, ABSOLUTE_WIDTH, y);
        }
        
        //std::cout << "Displaying Lines!" << std::endl;
        SDL_RenderPresent(renderer);
        
        // display blocks
        for (int i = 0; i < DISPLAY_WIDTH; i ++) {
            for (int j = 0; j < DISPLAY_HEIGHT; j ++) {
                if (displayGrid[i][j] == 1) {
                    WriteBlock(i, j, 1);
                } else {
                    WriteBlock(i, j, 0);
                }
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    

    
}


#endif // !DISPLAY_H
*/



void Emulator::WriteBlock(int x, int y, int val) {
    
    // writes block to white (val = 1) or black (val = 0)
    
    int x_start_coord = x * SPACING;
    int y_start_coord = y * SPACING;

    
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



#endif // !DISPLAY_H
