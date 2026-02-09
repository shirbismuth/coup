# 🎮 Coup — C++ GUI Game (SFML)

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](#) [![Build](https://img.shields.io/badge/build-Makefile-success)](#) [![GUI](https://img.shields.io/badge/GUI-SFML-informational)](#)

A graphical implementation of the board game **Coup**, written in **C++17** using the **SFML** library. Features role-based game logic, strategic bluffing mechanics, and a complete GUI implementation.

---

## 🎯 Features

- **Multiplayer Turn-Based Gameplay** – Support for multiple players with distinct roles
- **Six Unique Characters** – Governor, Spy, Baron, General, Judge, Merchant with special abilities
- **Graphical User Interface** – Built with SFML for interactive gameplay
- **Game Logic** – Coin management, action resolution, blocking mechanics, and win conditions
- **Automated Tests** – Unit tests using doctest framework
- **Modular Architecture** – Separation of game logic and GUI layers for extensibility

---

## 📖 Project Overview

Coup is a strategic bluffing card game where players compete to be the last one standing. Players take turns performing actions, bluffing about their roles, and eliminating opponents through coups. Each character has unique abilities that affect gameplay strategy.

This implementation provides:
- Complete game rules and mechanics
- Visual representation of game state
- Turn-based action system
- Role-specific abilities and blocking mechanisms

---

## 👥 Roles & Characters

The game includes six distinct playable characters, each with unique abilities:

- **Governor** – Receives additional coins from tax actions and can block foreign aid
- **Spy** – Inspects opponent hands and blocks arrest actions
- **Baron** – Invests coins and receives compensation when challenged
- **General** – Blocks coup attempts and can challenge opponents
- **Judge** – Blocks bribes and penalizes attackers with temporary penalties
- **Merchant** – Gains passive income and has enhanced trading capabilities

Each role is implemented as a separate class inheriting from the `Player` base class.

---

## 📁 Project Structure

```
coupfix/
├── src/                  # C++ source implementation files
├── include/              # Header files
├── assets/               # Fonts and image resources
├── tests/                # Unit tests
├── makefile              # Build configuration
└── README.md
```

---

## 🛠️ Technologies

- **Language:** C++17
- **GUI Framework:** SFML (Simple and Fast Multimedia Library)
- **Build System:** GNU Make
- **Testing Framework:** doctest

---

## 🚀 Build and Run

### Prerequisites (Linux / WSL)

Install required build tools and SFML library:

```bash
sudo apt update
sudo apt install build-essential libsfml-dev
```

### Build

Compile all targets:

```bash
make
```

This builds three executables:
- `coup_gui` – Main game with GUI
- `demo` – Demo/example program
- `test` – Unit test suite

### Run

**Start the game:**

```bash
./coup_gui
```

**Run the demo:**

```bash
./demo
```

**Execute tests:**

```bash
./test
```

### Clean

Remove build artifacts:

```bash
make clean
```

---

## 🧪 Testing

The project uses **doctest** for unit testing. Tests are located in `tests/tests.cpp` and cover core game mechanics including:
- Game initialization
- Player actions
- Role-specific abilities
- Blocking mechanisms
- Win condition logic

Run tests with:

```bash
./test
```

---

## 💡 Design Notes

- **Object-Oriented Design:** Each role is a subclass of `Player` with role-specific behavior
- **Separation of Concerns:** Game logic is independent from GUI rendering
- **Extensibility:** New roles can be added by extending the `Player` class
- **State Management:** Game state is managed centrally in the `Game` class
- **GUI Integration:** SFML handles rendering and user input

---

## ✍️ Author

Shir Bismuth

