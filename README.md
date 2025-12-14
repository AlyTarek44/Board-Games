# 🎮 Board Games Collection (C++)
---
## 👥 Team & Work Breakdown


| Name | Games |
|------|-------|
| [Aly Tarek](https://github.com/AlyTarek44) | 5×5 Tic-Tac-Toe, Word Tic-Tac-Toe |
| [Mohamed Farag](https://github.com/Mohamed-Farag911) | 4×4 Tic-Tac-Toe, Pyramid Tic-Tac-Toe |
| [Abdelrahman Hamdy](https://github.com/hamdy339) | SUS Tic-Tac-Toe, Four-in-a-Row |
| [Mohamed Khalid](https://github.com/mohamedkhalid98) | Misère Tic-Tac-Toe, Diamond Tic-Tac-Toe |
| **Group Contributions** | NUMTTT, Obstacles TTT, Infinity TTT, Ultimate TTT, Memory TTT |


👨‍🏫 *Supervisor:*  
*Dr. Mohamed ElRamly*

---

## 🔗 Project Links

**🎥 Project Demo Video:** [link](https://drive.google.com/file/d/1Q7n2Zk-cc5Y7nbHRNX4oTZ82KtMl7NZQ/view)  
**📄 Doxygen Documentation:** [Link](https://mohamed-farag911.github.io/Games.github.io/)  

---

## 📘 Project Overview

This repository contains a *comprehensive collection of board and puzzle games implemented in C++, developed as part of **CS213 – Object-Oriented Programming, Faculty of Computers and Artificial Intelligence, Cairo University (2025–2026).***

The project is built on a *shared, reusable game framework* and showcases advanced usage of *Object-Oriented Programming (OOP)* concepts such as inheritance, polymorphism, abstraction, and encapsulation across many different game variants.

A full academic report describing the design, AI logic, and OOP analysis of each game is included and served as the primary reference for this README.

---

## 🎯 Educational Objectives

* Apply OOP principles in a large-scale, multi-file C++ project
* Design reusable and extensible class hierarchies
* Implement different board geometries and rule sets
* Develop AI strategies (heuristic, Minimax, Alpha–Beta pruning)
* Practice teamwork, code reviews, and software documentation

---

## 🧩 Implemented Games

### Individual Games

#### 🟢 SUS Tic-Tac-Toe

* Score-based 3×3 game (S–U–S sequences)
* Heuristic AI that prioritizes scoring and blocking

#### 🔵 Four-in-a-Row

* 6×7 grid with gravity mechanics
* Minimax AI with Alpha–Beta pruning

#### 🟣 Five-by-Five Tic-Tac-Toe

* 5×5 board, win by forming 3-in-a-row
* Minimax AI with board evaluation

#### 🟡 Word Tic-Tac-Toe

* Players place letters instead of symbols
* Win by forming valid dictionary words

#### 🔴 Misère Tic-Tac-Toe

* Inverted logic: forming 3-in-a-row causes a loss
* Backtracking AI that avoids "suicide" moves

#### 🔶 Diamond Tic-Tac-Toe

* 5×5 grid restricted to a diamond shape
* Unique win condition: line of 3 *and* line of 4 simultaneously
* Optimized Minimax with memoization

#### 🔺 Pyramid Tic-Tac-Toe

* Pyramid-shaped board (1–3–5)
* Custom win paths and Minimax AI

#### 🔁 4×4 Sliding Tic-Tac-Toe

* Tokens are moved instead of placed
* Uses a custom Move class (source → destination)
* Depth-limited Minimax AI

---

### Group Games

#### 🔢 Numerical Tic-Tac-Toe (NUMTTT)

* Uses numbers instead of symbols
* Win by achieving a sum of 15
* Demonstrates template flexibility with Board<int>

#### ⛔ Obstacles Tic-Tac-Toe

* Board contains blocked cells (#)
* Modified move validation and win logic

#### ♾ Infinity Tic-Tac-Toe

* Sliding-window Tic-Tac-Toe
* Oldest moves are removed dynamically

#### 🧠 Memory Tic-Tac-Toe

* Board is hidden from the player
* Only the current move is briefly revealed

#### 🧩 Ultimate Tic-Tac-Toe

* 9×9 grid composed of 3×3 sub-boards
* Meta-board win tracking

---

## 🏗 Architecture & Design

### Core Framework

All games are built on top of shared abstract base classes:

* Board<T> – Manages board state and rules
* Move<T> – Encapsulates player actions
* Player<T> – Human and AI players
* UI<T> – Console-based interaction layer

### OOP Principles Demonstrated

* *Inheritance:* All games extend common base classes
* *Polymorphism:* Virtual functions allow different game rules using the same interface
* *Abstraction:* Complex logic (geometry, scoring, AI) is hidden behind simple methods
* *Encapsulation:* Game state and helper logic are kept private and accessed safely

---

## 🤖 Artificial Intelligence

Different games employ different AI strategies:

* Heuristic evaluation (SUS, Word TTT)
* Minimax search
* Alpha–Beta pruning
* Memoization & move ordering
* Depth-limited backtracking for complex move spaces

AI behavior is customized per game to match its rule complexity and board size.

---

## ▶ Build & Run Instructions

### Requirements

* C++ compiler supporting modern standards (GCC / Clang / MSVC)
* CMake

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
./OOP_Games
```


---

## 📜 License

This project is intended for *educational and academic use*. Free to use and modify for learning purposes © **2025**.

---
