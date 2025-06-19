CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = \
    Game.cpp \
    Player.cpp \
    Merchant.cpp \
    Spy.cpp \
    General.cpp \
    Judge.cpp \
    Baron.cpp \
    Governor.cpp

OBJ = $(SRC:.cpp=.o)

# GUI target
GUI_TARGET = coup_gui
GUI_OBJ = GUI.o $(OBJ)

# Demo target
DEMO_TARGET = demo
DEMO_OBJ = Demo.o $(OBJ)

# Tests target
TEST_TARGET = test
TEST_OBJ = tests.o $(OBJ)

.PHONY: all gui demo test clean

default: all

all: gui demo test

gui: $(GUI_TARGET)

$(GUI_TARGET): GUI.o $(OBJ)
	$(CXX) GUI.o $(OBJ) -o $@ $(LDFLAGS)

demo: $(DEMO_TARGET)

$(DEMO_TARGET): Demo.o $(OBJ)
	$(CXX) Demo.o $(OBJ) -o $@ $(LDFLAGS)

test: $(TEST_TARGET)

$(TEST_TARGET): tests.o $(OBJ)
	$(CXX) tests.o $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) GUI.o Demo.o tests.o $(GUI_TARGET) $(DEMO_TARGET) $(TEST_TARGET)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./test


