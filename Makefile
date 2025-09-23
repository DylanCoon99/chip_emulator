

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -Weffc++ -Wconversion -Wsign-conversion -std=c++17
SOURCES = ./src/main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = emulator

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)


