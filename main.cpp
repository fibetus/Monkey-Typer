/**
 * @file main.cpp
 * @brief Entry point for the Monkey Typer game
 * @date 2024
 *
 * @mainpage Monkey Typer - A Typing Game
 *
 * @section intro_sec Introduction
 * Monkey Typer is an SFML-based typing game where players must type
 * falling words before they reach the edge of the screen.
 *
 * @section features_sec Features
 * - Multiple font options
 * - Progressive difficulty
 * - High score tracking
 * - Save/Load game functionality
 * - Sound effects
 *
 * @section arch_sec Architecture
 * The game uses the State Pattern to manage different screens:
 * - MenuState: Main menu
 * - PlayingState: Active gameplay
 * - GameOverState: Game over screen
 * - FontSelectState: Font selection menu
 */

#include "Game.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Main entry point
 * @return 0 on success, non-zero on error
 */
auto main() -> int {
  try {
    Game game;
    game.run();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }
}
