#ifndef UTIL_H
#define UTIL_H



int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    return -1; // Invalid hex character
}


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


void AddBinaryNumbers(uint8_t xValue, uint8_t yValue, int* sum, int* carry) {
    
    // iterate over 8 bits
    for (int i = 0; i < 8; i ++) {
        int xBit = (xValue >> i) & 0x1;
        int yBit = (yValue >> i) & 0x1;
        
        // add the bits
        int s = xBit ^ yBit ^ *carry;
        *carry = (xBit & yBit) | (xBit & *carry) | (yBit & *carry);
        
        *sum |= (s << i);
         
    }
    
}




// Keypad notes
// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F

// On the actual keyboard
// 1 2 3 4
// Q W E R
// A S D F
// Z X C V





#endif // !UTIL_H
