README


g++ -Wall -Werror -Weffc++ -Wextra -Wconversion -Wsign-conversion main.cpp -o main


00E0 (Clear screen)

(Point I register to the ibm sprite)

(Set V0 register to 12)

(Set V1 register to 8)

D v0 v1 15 (Display at (x, y) -> (v0, v1) with height of 15)


# Note: 0x2a in 2-ibm-logo.ch8 is the start of the ibm sprite (the code seems to start at 0x0)
