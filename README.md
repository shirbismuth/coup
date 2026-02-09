# 🎮 Coup — C++ GUI Game (SFML)

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](#)
[![Build](https://img.shields.io/badge/build-Makefile-success)](#)
[![GUI](https://img.shields.io/badge/GUI-SFML-informational)](#)

A graphical implementation of the board game **Coup**, written in **C++17** using **SFML**.  
Focused on clean **OOP design**, **role-based mechanics**, and an interactive **GUI**.

---

## ✨ Features

- Full **GUI gameplay** (SFML)
- Clear separation between **game logic** and **UI**
- Multiple **roles** with unique abilities
- **Unit tests** using *doctest*
- Clean project structure: `src/`, `include/`, `assets/`, `tests/`

---

## 🧩 Project Overview

**Coup** is a strategic bluffing game where players collect coins, perform actions, block opponents, and eliminate others using coups.  
The goal is to be the **last remaining player**.

---

## 🧑‍🎭 Roles

- **Governor** — gains more from tax and can block taxes  
- **Spy** — can inspect opponents and block arrest actions  
- **Baron** — can invest coins and gets compensation when sanctioned  
- **General** — can block coup attempts  
- **Judge** — can block bribes and penalize attackers  
- **Merchant** — passive income & special benefits  

> Each role is implemented as a separate class.

---

## 🗂️ Project Structure

```text
coup/
├── src/        # C++ source files (.cpp)
├── include/    # Header files (.hpp)
├── assets/     # Fonts & images for the GUI
├── tests/      # Unit tests (doctest)
├── makefile    # Build configuration
└── README.md
