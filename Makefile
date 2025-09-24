
DEBUG_FLAG ?= 0

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Weffc++ -Wconversion -Wsign-conversion -std=c++17
LIBS = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -framework OpenGL
SOURCES = ./src/main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = emulator

ifeq ($(DEBUG_FLAG), 1)
	CXXFLAGS += -g -O0
else
	CXXFLAGS += -O2
endif


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)


