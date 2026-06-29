/*
 * ╔══════════════════════════════════════════════════════════╗
 * ║          NUMBER GUESSING GAME  —  C++ Console           ║
 * ║                  Dynamic & Interactive                  ║
 * ╚══════════════════════════════════════════════════════════╝
 *
 *  Features:
 *   + True random number generation (seeded with time)
 *   + 3 difficulty levels  (Easy / Medium / Hard)
 *   + Attempt tracking per round
 *   + Best-score / high-score board  (current session)
 *   + Hot-Cold proximity hints
 *   + Input validation (non-numeric, out-of-range)
 *   + Replay loop — play as many rounds as you like
 *   + Coloured ANSI output for Windows / Unix terminals
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>

// ─── ANSI colour helpers ──────────────────────────────────────────────────────
#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  // ENABLE_VIRTUAL_TERMINAL_PROCESSING may not be defined in older SDKs
  #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  #endif
  static void enableAnsi() {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD dwMode = 0;
      GetConsoleMode(hOut, &dwMode);
      SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  }
#else
  static void enableAnsi() {}
#endif

// Colour codes
const std::string RESET   = "\033[0m";
const std::string BOLD    = "\033[1m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string BLUE    = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN    = "\033[36m";

// ─── Utility helpers ──────────────────────────────────────────────────────────
void printLine(char ch = '-', int width = 60) {
    std::cout << CYAN;
    for (int i = 0; i < width; ++i) std::cout << ch;
    std::cout << RESET << "\n";
}

void printBanner() {
    std::cout << "\n";
    printLine('=');
    std::cout << BOLD << BLUE
              << "  ███╗  ██╗██╗   ██╗███╗  ███╗██████╗ ███████╗██████╗ \n"
              << "  ████╗ ██║██║   ██║████╗████║██╔══██╗██╔════╝██╔══██╗\n"
              << "  ██╔██╗██║██║   ██║██╔████╔██║██████╔╝█████╗  ██████╔╝\n"
              << "  ██║╚████║██║   ██║██║╚██╔╝██║██╔══██╗██╔══╝  ██╔══██╗\n"
              << "  ██║ ╚███║╚██████╔╝██║ ╚═╝ ██║██████╔╝███████╗██║  ██║\n"
              << "  ╚═╝  ╚══╝ ╚═════╝ ╚═╝     ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝\n"
              << RESET;
    std::cout << BOLD << YELLOW
              << "        G U E S S I N G   G A M E\n"
              << RESET;
    printLine('=');
    std::cout << "\n";
}

// ─── Difficulty structure ─────────────────────────────────────────────────────
struct Difficulty {
    std::string name;
    int         minVal;
    int         maxVal;
    int         maxAttempts;   // 0 = unlimited
    std::string colour;
};

const Difficulty LEVELS[] = {
    { "Easy",   1,  50, 10, GREEN  },
    { "Medium", 1, 100,  7, YELLOW },
    { "Hard",   1, 200,  5, RED    }
};
const int NUM_LEVELS = 3;

// ─── Session high-score board ─────────────────────────────────────────────────
struct ScoreEntry {
    std::string playerName;
    std::string level;
    int         attempts;
    int         secretNumber;
};

std::vector<ScoreEntry> scoreboard;

void showScoreboard() {
    if (scoreboard.empty()) {
        std::cout << YELLOW << "  No scores yet this session.\n" << RESET;
        return;
    }
    std::vector<ScoreEntry> sorted = scoreboard;
    std::sort(sorted.begin(), sorted.end(),
              [](const ScoreEntry& a, const ScoreEntry& b){
                  return a.attempts < b.attempts;
              });

    printLine();
    std::cout << BOLD << CYAN << "  ** SESSION SCOREBOARD **\n" << RESET;
    printLine();
    std::cout << BOLD
              << std::left
              << "  " << std::setw(6)  << "Rank"
              << std::setw(16) << "Player"
              << std::setw(10) << "Level"
              << std::setw(10) << "Attempts"
              << std::setw(10) << "Answer"
              << "\n" << RESET;
    printLine('-');
    for (int i = 0; i < (int)sorted.size(); ++i) {
        std::string rank;
        if      (i == 0) rank = "[1st]";
        else if (i == 1) rank = "[2nd]";
        else if (i == 2) rank = "[3rd]";
        else             rank = "[" + std::to_string(i + 1) + "th]";

        std::cout << "  " << std::left
                  << std::setw(6)  << rank
                  << std::setw(16) << sorted[i].playerName
                  << std::setw(10) << sorted[i].level
                  << std::setw(10) << sorted[i].attempts
                  << std::setw(10) << sorted[i].secretNumber
                  << "\n";
    }
    printLine();
}

// ─── Safe integer input ───────────────────────────────────────────────────────
bool safeReadInt(int& value) {
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

// ─── Proximity / Hot-Cold hint ────────────────────────────────────────────────
std::string proximityHint(int guess, int secret, int range) {
    int diff = std::abs(guess - secret);
    double pct = (double)diff / range;
    if (pct <= 0.05) return BOLD + RED    + ">>> BURNING HOT! <<<" + RESET;
    if (pct <= 0.12) return BOLD + RED    + "Very Hot"             + RESET;
    if (pct <= 0.25) return        YELLOW + "Warm"                 + RESET;
    if (pct <= 0.40) return        CYAN   + "Cool"                 + RESET;
    return                          BLUE  + "Ice Cold"             + RESET;
}

// ─── Direction hint ───────────────────────────────────────────────────────────
std::string directionHint(int guess, int secret) {
    return (guess < secret)
        ? GREEN + "Go HIGHER ^" + RESET
        : RED   + "Go LOWER  v" + RESET;
}

// ─── Choose difficulty ────────────────────────────────────────────────────────
int chooseDifficulty() {
    std::cout << BOLD << "\n  Choose Difficulty:\n" << RESET;
    for (int i = 0; i < NUM_LEVELS; ++i) {
        std::cout << "  " << BOLD << (i + 1) << ". " << RESET
                  << LEVELS[i].colour << LEVELS[i].name << RESET
                  << "  [" << LEVELS[i].minVal << " - " << LEVELS[i].maxVal << "]"
                  << "  max attempts: ";
        if (LEVELS[i].maxAttempts == 0)
            std::cout << GREEN << "unlimited" << RESET;
        else
            std::cout << YELLOW << LEVELS[i].maxAttempts << RESET;
        std::cout << "\n";
    }
    std::cout << "  Your choice (1-" << NUM_LEVELS << "): ";

    int choice;
    while (true) {
        if (safeReadInt(choice) && choice >= 1 && choice <= NUM_LEVELS)
            break;
        std::cout << RED << "  Invalid! Enter 1-" << NUM_LEVELS << ": " << RESET;
    }
    return choice - 1;  // 0-based index
}

// ─── Single game round ────────────────────────────────────────────────────────
void playRound(const std::string& playerName) {
    int levelIdx = chooseDifficulty();
    const Difficulty& lvl = LEVELS[levelIdx];

    // True random number
    int secret = lvl.minVal + rand() % (lvl.maxVal - lvl.minVal + 1);
    int range  = lvl.maxVal - lvl.minVal + 1;

    printLine();
    std::cout << BOLD << lvl.colour
              << "  [" << lvl.name << "] "
              << RESET << "I've picked a number between "
              << BOLD << lvl.minVal << RESET << " and "
              << BOLD << lvl.maxVal << RESET << ".\n";

    if (lvl.maxAttempts > 0)
        std::cout << "  You have " << BOLD << YELLOW
                  << lvl.maxAttempts << " attempts." << RESET << "\n";
    else
        std::cout << "  " << GREEN << "Unlimited attempts - take your time!\n" << RESET;
    printLine();

    int attempts = 0;
    bool won     = false;

    while (true) {
        // Check attempt limit
        if (lvl.maxAttempts > 0 && attempts >= lvl.maxAttempts) {
            std::cout << "\n" << RED << BOLD
                      << "  !! Out of attempts! The number was "
                      << secret << ".\n" << RESET;
            break;
        }

        int remaining = (lvl.maxAttempts > 0) ? (lvl.maxAttempts - attempts) : -1;
        if (remaining > 0)
            std::cout << CYAN << "  [" << remaining << " left] " << RESET;
        else
            std::cout << "  > ";

        std::cout << "Your guess: ";
        int guess;
        if (!safeReadInt(guess)) {
            std::cout << RED << "  Please enter a valid integer.\n" << RESET;
            continue;
        }
        if (guess < lvl.minVal || guess > lvl.maxVal) {
            std::cout << YELLOW << "  Out of range! Guess between "
                      << lvl.minVal << " and " << lvl.maxVal << ".\n" << RESET;
            continue;
        }

        ++attempts;

        if (guess == secret) {
            won = true;
            break;
        }

        // Hints
        std::cout << "      " << directionHint(guess, secret)
                  << "   " << proximityHint(guess, secret, range) << "\n";
    }

    if (won) {
        printLine('*');
        std::cout << BOLD << GREEN
                  << "\n  >> Brilliant, " << playerName << "! You guessed "
                  << BOLD << YELLOW << secret << RESET << GREEN
                  << " in " << attempts << " attempt"
                  << (attempts == 1 ? "" : "s") << "!\n\n" << RESET;

        // Star rating
        double ratio = (lvl.maxAttempts > 0)
            ? (double)attempts / lvl.maxAttempts
            : (double)attempts / 10.0;
        std::string stars;
        if      (ratio <= 0.4) stars = "***** (5 Stars - PERFECT!)";
        else if (ratio <= 0.6) stars = "**** (4 Stars - Great!)";
        else if (ratio <= 0.8) stars = "*** (3 Stars - Good!)";
        else                   stars = "** (2 Stars - Keep practicing!)";
        std::cout << "  Performance: " << YELLOW << stars << RESET << "\n";
        printLine('*');

        scoreboard.push_back({ playerName, lvl.name, attempts, secret });
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    enableAnsi();
    srand(static_cast<unsigned int>(time(nullptr)));  // true random seed

    printBanner();

    // Get player name
    std::cout << BOLD << "  Enter your name: " << RESET;
    std::string playerName;
    std::getline(std::cin, playerName);
    if (playerName.empty()) playerName = "Player";

    std::cout << "\n" << GREEN << "  Welcome, " << BOLD << playerName
              << RESET << GREEN << "! Let's play!\n" << RESET;

    // Main loop
    char again = 'y';
    while (again == 'y' || again == 'Y') {
        playRound(playerName);

        std::cout << "\n";
        showScoreboard();

        std::cout << "\n" << BOLD
                  << "  Play again? (y/n): " << RESET;
        std::string input;
        std::getline(std::cin, input);
        again = input.empty() ? 'n' : input[0];
    }

    // Farewell
    printLine('=');
    std::cout << BOLD << MAGENTA
              << "\n  Thanks for playing, " << playerName << "! Goodbye!\n\n"
              << RESET;
    printLine('=');
    return 0;
}
