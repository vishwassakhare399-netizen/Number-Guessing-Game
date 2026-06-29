# 🎯 Number Guessing Game

> **A fully interactive, dynamic, console-based C++ Number Guessing Game** — featuring difficulty levels, smart hints, a session scoreboard, star ratings, and colourful ANSI output.

[![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-informational?style=for-the-badge)](https://github.com)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Active-brightgreen?style=for-the-badge)]()

---

## 📋 Table of Contents

1. [About the Project](#-about-the-project)
2. [Features](#-features)
3. [How the Game Works](#-how-the-game-works)
4. [Prerequisites](#-prerequisites)
5. [Installation & Setup](#-installation--setup)
   - [Windows](#-windows)
   - [Linux / macOS](#-linux--macos)
6. [Compilation](#-compilation)
7. [Running the Game](#-running-the-game)
8. [Gameplay Guide](#-gameplay-guide)
9. [Project Structure](#-project-structure)
10. [Contributing](#-contributing)
11. [License](#-license)

---

## 🚀 About the Project

This is **not** a boring static game where the number is hardcoded. Every single round generates a **truly random secret number** seeded from the system clock (`time(nullptr)`), so no two games are ever the same.

The game is written in **pure standard C++** with zero external dependencies — just compile and play!

---

## ✨ Features

| Feature | Description |
|---|---|
| 🎲 **True Randomness** | Secret number seeded from system clock — never repeats |
| 🏔 **3 Difficulty Levels** | Easy (1–50, 10 tries) · Medium (1–100, 7 tries) · Hard (1–200, 5 tries) |
| 🌡 **Hot-Cold Hints** | Proximity feedback: 🔥 Burning Hot → ❄ Ice Cold |
| 📈 **Direction Hints** | "Go HIGHER" / "Go LOWER" after every wrong guess |
| 🏆 **Session Scoreboard** | Tracks all rounds, sorted by fewest attempts |
| ⭐ **Star Rating** | Performance rated 2–5 stars based on efficiency |
| 🔒 **Input Validation** | Handles non-numeric input and out-of-range guesses gracefully |
| 🔁 **Replay Loop** | Play unlimited rounds without restarting the program |
| 🎨 **ANSI Colour UI** | Vibrant coloured terminal output (Windows 10+ & Unix) |
| 👤 **Player Name** | Personalised messages throughout the game |

---

## 🎮 How the Game Works

```
1. Enter your name
2. Choose a difficulty level
3. The game picks a secret random number
4. Guess the number:
   - After each wrong guess you get:
       (a) Direction hint  → go higher / lower
       (b) Proximity hint  → how close you are (hot/cold)
   - Track remaining attempts in the prompt
5. Win → get a star rating + scoreboard entry
6. Lose (out of attempts) → see the secret number
7. Choose to play again or quit
8. Final session scoreboard displayed on exit
```

---

## 🔧 Prerequisites

You only need a **C++ compiler** that supports **C++11 or later** (C++17 recommended).

| Tool | Minimum Version | Download |
|---|---|---|
| **g++ (MinGW / GCC)** | 7.0+ | [winlibs.com](https://winlibs.com/) or [MinGW-w64](https://www.mingw-w64.org/) |
| **MSVC (Visual Studio)** | VS 2017+ | [visualstudio.microsoft.com](https://visualstudio.microsoft.com/) |
| **Clang** | 6.0+ | [releases.llvm.org](https://releases.llvm.org/) |

> **Windows Users**: The recommended approach is to install **MinGW-w64** (g++) and use it from the command prompt or PowerShell.

---

## 📦 Installation & Setup

### 🪟 Windows

#### Step 1 — Install MinGW-w64 (g++)

1. Download the installer from **[winlibs.com](https://winlibs.com/)** — choose the latest GCC release for Win64.
2. Extract the archive (e.g., to `C:\mingw64`).
3. Add the `bin` folder to your **PATH** environment variable:
   - Press `Win + X` → **System** → **Advanced system settings** → **Environment Variables**
   - Under *System variables*, find `Path`, click **Edit**, then **New**
   - Add: `C:\mingw64\bin` (or wherever you extracted MinGW)
   - Click **OK** on all dialogs
4. Verify installation — open **PowerShell** or **CMD**:
   ```powershell
   g++ --version
   ```
   You should see something like `g++ (x86_64-posix-seh) 13.x.x`.

#### Step 2 — Clone the Repository

Open **PowerShell** and run:

```powershell
git clone https://github.com/vishwassakhare399-netizen/Number-Guessing-Game.git
cd "Number-Guessing-Game"
```

> If you don't have Git: [git-scm.com/downloads](https://git-scm.com/downloads)

---

### 🐧 Linux / macOS

#### Step 1 — Install g++

**Ubuntu / Debian:**
```bash
sudo apt update
sudo apt install build-essential
```

**Fedora / RHEL:**
```bash
sudo dnf install gcc-c++
```

**macOS (Homebrew):**
```bash
brew install gcc
```

**Verify:**
```bash
g++ --version
```

#### Step 2 — Clone the Repository

```bash
git clone https://github.com/vishwassakhare399-netizen/Number-Guessing-Game.git
cd Number-Guessing-Game
```

---

## 🔨 Compilation

Navigate into the project directory and compile with one command.

### Windows (PowerShell / CMD)

```powershell
g++ -std=c++17 -O2 -Wall -o number_guessing_game number_guessing_game.cpp
```

### Linux / macOS (Terminal)

```bash
g++ -std=c++17 -O2 -Wall -o number_guessing_game number_guessing_game.cpp
```

> **Flag Explanations:**
> | Flag | Meaning |
> |---|---|
> | `-std=c++17` | Use the C++17 standard |
> | `-O2` | Optimisation level 2 (faster binary) |
> | `-Wall` | Enable all compiler warnings |
> | `-o number_guessing_game` | Name the output executable |

---

## ▶ Running the Game

### Windows

```powershell
.\number_guessing_game.exe
```

### Linux / macOS

```bash
./number_guessing_game
```

---

## 🕹 Gameplay Guide

### Difficulty Levels

| Level | Range | Max Attempts | Tip |
|---|---|---|---|
| 🟢 **Easy** | 1 – 50 | 10 | Great for beginners |
| 🟡 **Medium** | 1 – 100 | 7 | The classic challenge |
| 🔴 **Hard** | 1 – 200 | 5 | For true guessers |

### Hint System

After every wrong guess you'll see **two hints**:

| Hint Type | Examples |
|---|---|
| **Direction** | `📈 Go HIGHER` / `📉 Go LOWER` |
| **Proximity** | `🔥 BURNING HOT!` (≤5%) · `🌡 Very Hot` · `☀ Warm` · `🌊 Cool` · `❄ Ice Cold` |

### Star Rating (on win)

| Stars | Condition |
|---|---|
| ⭐⭐⭐⭐⭐ | Used ≤ 40% of allowed attempts |
| ⭐⭐⭐⭐ | Used ≤ 60% |
| ⭐⭐⭐ | Used ≤ 80% |
| ⭐⭐ | Used more than 80% |

### Tips to Win

- **Binary search** is your best friend: always guess the midpoint of the remaining range.
- Watch the **proximity hint** — if you jump from "Warm" to "Ice Cold" you overshot.
- On **Hard** mode, start at **100** (midpoint of 1–200) and halve the range each time.

---

## 📁 Project Structure

```
Number-Guessing-Game/
│
├── number_guessing_game.cpp   ← Main source code (entire game)
├── README.md                  ← This documentation file
└── LICENSE                    ← MIT License
```

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

1. **Fork** the project
2. Create your feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a **Pull Request**

**Ideas for future contributions:**
- Save scores to a file (persistent high-score board)
- Multiplayer mode (two players alternating)
- Custom difficulty (user-defined range and attempts)
- Timer challenge mode

---

## 📄 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for more information.

---

## 👨‍💻 Author

**Vishwas Sakhare**
- GitHub: [@vishwassakhare399-netizen](https://github.com/vishwassakhare399-netizen)

---

<p align="center">Made with ❤️ and C++</p>
