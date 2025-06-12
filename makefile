CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = \
    GUI.cpp \
    Game.cpp \
    Player.cpp \
    Merchant.cpp \
    Spy.cpp \
    General.cpp \
    Judge.cpp \
    Baron.cpp \
    Governor.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = coup_gui

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
