#include <iostream>
#include "../include/Emulator.h"
#include "../include/Display.h"
#include <chrono>
#include <thread>


/* CHIP 8 Emulator Notes

4k memory (4096 bytes)

Registers ()


Infinite loop [ fetch -> decode -> execute ] cycles

	* Fetch: grabs instruction that the PC (program counter) points to
		* Every instruction is 2 bytes so read 2 successive bytes from memory
		and combine them into one 16 bit instruction
		* increment the PC by 2 

	* Decode: determines what the emulator should do (what kind of instruction 
	is it; which registers are affected; etc.)
		* first nibble (half byte) tells you what kind of instruction it is.
		* rest of nibbles could have different meanings depending on the kind of instruction
		* 

	* Execute: perform the instruction
		* 





Instruction Set
	Instruction Structure
		X:   2nd nibble;     represents a 16 bit register VX (V0 - VF)
		Y:   3rd nibble;     represents a 16 bit register VY (V0 - VF)
		N:   4th nibble;     represents a 4 bit number
		NN:  2nd byte;       represents a 8 bit number
		NNN: last 3 nibbles; represents a 12 bit number



	Start with these basic instructions
	* 00E0 (clear screen)
	* 1NNN (jump) makes sense because NNN is 12 bits; 4096 bytes -> 2^12 bytes (memory is addressed with 12 bits)
	* 6XNN (set register VX)
	* 7XNN (add value to register VX)
	* ANNN (set index register I)
	* DXYN (display draw)
 
 
    IBM logo code (and explanation)
 
     00e0 (clear screen)
     a22a (set I register to 22a)
     600c (set register V0 to c)
     6108 (set register V1 to 8)
     d01f (display at coord (x = V0, y = V1) height = f)
     7009 (add 9 to V0)
     a239 (set I register to 239)
     d01f (display at coord (x = V0, y = V1) height = f)
     a248 (set I register to 248)
     7008 (add 8 to V0)
     d01f (display at coord (x = V0, y = V1) height = f)
     7004 (add 4 to V0)
     a257 (set I register to 257)
     d01f (display at coord (x = V0, y = V1) height = f)
     7008 (add 8 to V0)
     a266 (set I register to 266)
     d01f (display at coord (x = V0, y = V1) height = f)
     7008 (add 8 to V0)
     a275 (set I register to 275)
     d01f (display at coord (x = V0, y = V1) height = f)
     1228 (jump to address 0x222)

 

	All instructions

	* 00E0 - clear screen
	* 1NNN - jump
	* 00EE and 2NNN subroutines
		- 2NNN calls the subroutine at NNN (function call); much like a jump instruction
		only it also pushes the PC to the stack so it can return when done
		- 00EE returns from the subroutine (pops the previous PC and sets the PC to it)
	* 3XNN, 4XNN, 5XY0, and 9XY0 skip conditionally
		- 

 
 

Timing (what speed should the loop run at?)

	* ~700 instructions per second



Have a struct containing the registers and their current values

malloc a chunk of memory (4kb), partition it


g++ -Wall -Werror -Weffc++ -Wextra -Wconversion -Wsign-conversion main.cpp -o main
 
 
 */







int main(int argc, char* argv[]) {

	/*
		Initialize memory, load the ROM file into memory, initialize control flow registers (PC)
	*/
    
    Emulator myEmulator;

    
    std::cout << myEmulator.addressSpaceSize << std::endl;

    
    if (argc < 2) {
        std::cerr << "Error: not enough arguments." << std::endl;
        return 1;
    }
    
    std::string programName = argv[1];
    
    
    myEmulator.LoadROM(programName);
    
    
    
    //Display myDisplay;
    
    //myDisplay.InitDisplay();
    
    //myDisplay.DrawGrid();
    
    //myEmulator.DisplayAddressSpace(1000); // for debugging
    
    myEmulator.Run();
    
    //myEmulator.DisplayRegisters();
    
    
    
    return 0;
	

}
