CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
LDFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system

GUI_TARGET  = coup_gui
DEMO_TARGET = demo
TEST_TARGET = test

CORE_SRC = \
  src/Game.cpp \
  src/Player.cpp \
  src/Merchant.cpp \
  src/Spy.cpp \
  src/General.cpp \
  src/Judge.cpp \
  src/Baron.cpp \
  src/Governor.cpp

CORE_OBJ = $(CORE_SRC:.cpp=.o)

GUI_OBJ  = src/GUI.o  $(CORE_OBJ)
DEMO_OBJ = src/Demo.o $(CORE_OBJ)
TEST_OBJ = tests/tests.o $(CORE_OBJ)

.PHONY: all gui demo test clean valgrind

all: gui demo test

gui: $(GUI_TARGET)
demo: $(DEMO_TARGET)
test: $(TEST_TARGET)

$(GUI_TARGET): $(GUI_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(DEMO_TARGET): $(DEMO_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Generic compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o tests/*.o $(GUI_TARGET) $(DEMO_TARGET) $(TEST_TARGET)

valgrind: test
	valgrind --leak-check=full --track-origins=yes ./$(TEST_TARGET)
