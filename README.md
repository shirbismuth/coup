# Coup – C++ GUI Game

A graphical implementation of the board game **Coup**, written in **C++** using the **SFML** library.

The project demonstrates object-oriented design, role-based game logic, and a graphical user interface.

---

## Project Description

Coup is a strategic bluffing game where each player controls a character with unique abilities.
Players collect coins, perform actions, block opponents, and eliminate other players using coups.
The goal is to be the last remaining player in the game.

This project includes a full GUI, game logic implementation, and unit tests.

---

## Characters

The game includes the following roles:

- **Governor** – Receives additional coins from tax actions and can block taxes  
- **Spy** – Can inspect opponents and block arrest actions  
- **Baron** – Can invest coins and receives compensation when sanctioned  
- **General** – Can block coup attempts  
- **Judge** – Can block bribes and penalize attackers  
- **Merchant** – Gains passive income and special benefits  

Each role is implemented as a separate class.

---

## Project Structure

coupfix/
├── src/ # C++ source files
├── include/ # Header files
├── assets/ # Fonts and images
├── tests/ # Unit tests
├── makefile # Build configuration
└── README.md


---

## Technologies

- C++17  
- SFML  
- GNU Make  
- doctest (unit testing)

---

## Build and Run

### Prerequisites (Linux / WSL)

```bash
sudo apt update
sudo apt install build-essential libsfml-dev
Build the project
make
Run the game
./coup_gui
Run tests
./test
Clean build files
make clean
Notes
The project separates game logic from the GUI layer

Designed to be easily extensible

Includes automated tests for core mechanics

Author
Shir Bismuth
