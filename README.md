# Coup-like Game Project

## Project Overview
This project is a C++ implementation of a Coup-like strategy board game for 2-6 players, featuring unique roles, turn-based mechanics, and a graphical user interface (GUI). The game enforces all rules, including turn order, coin requirements, and special abilities for each role. The project includes a demo, a comprehensive test suite, and memory safety checks.

## Directory and File Structure
- **main.cpp**: Entry point for the demo version of the game.
- **Demo.cpp**: Contains the demonstration logic for running a sample game.
- **GUI.cpp**: Implements the graphical user interface for interactive play.
- **tests.cpp**: Contains unit tests for all game logic and player actions.
- **makefile**: Build system supporting demo, GUI, tests, and valgrind memory checks.
- **Player.hpp/cpp**: Base class for all player types and general player logic.
- **Game.hpp/cpp**: Manages the game state, turn order, player actions, and win conditions.
- **General.hpp/cpp, Spy.hpp/cpp, Judge.hpp/cpp, Governor.hpp/cpp, Baron.hpp/cpp, Merchant.hpp/cpp**: Implement unique player roles and their special abilities.
- **doctest.h**: Header-only testing framework.
- **ARIAL.TTF, Junicode.ttf, stone_texture.png.jpg**: Assets for the GUI.

## Class and Module Overview
- **Game**: Manages players, turn order, actions, and win conditions.
- **Player (and subclasses)**: Implements player logic and unique abilities for each role:
  - *General*: Can block coups and recover coins from arrests.
  - *Spy*: Can view coins and block arrests.
  - *Judge*: Can block bribes and penalize sanctions.
  - *Governor*: Enhanced tax and can block others' tax actions.
  - *Baron*: Investment ability and sanction compensation.
  - *Merchant*: Bonus coins and special arrest penalty.
- **GUI**: Provides a graphical interface for playing the game interactively.
- **Demo**: Runs a sample game for demonstration purposes.
- **Tests**: Comprehensive unit tests for all game logic and player actions.

## Testing and Quality
- All logic is covered by unit tests in `tests.cpp` using the `doctest` framework.
- Run `make test` to execute the test suite.
- Run `make valgrind` to check for memory leaks.
- The codebase is modular, well-documented, and follows best practices for clarity and maintainability.


---

