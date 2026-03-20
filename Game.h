/**
 * @file Game.h
 * @brief Main game controller class
 * @date 2024
 *
 * Manages game lifecycle, state transitions, and resource coordination.
 */

#ifndef MONKEY_TYPER_GAME_H
#define MONKEY_TYPER_GAME_H

#include "IGameState.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>


/**
 * @brief Main game controller using State Pattern
 *
 * This class manages the game window, state transitions, and coordinates
 * between different game states (Menu, Playing, GameOver, FontSelect).
 * Uses smart pointers for RAII-compliant resource management.
 */
class Game {
public:
  /**
   * @brief Available game states
   */
  enum class StateType { Menu, Playing, GameOver, FontSelect };

  /**
   * @brief Construct the game and initialize resources
   */
  Game();

  /**
   * @brief Default destructor (smart pointers handle cleanup)
   */
  ~Game() = default;

  // Disable copy operations
  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  /**
   * @brief Run the main game loop
   *
   * Continuously updates and renders until window is closed.
   */
  void run();

  /**
   * @brief Change to a new game state
   * @param stateType The state to transition to
   */
  void changeState(StateType stateType);

  /**
   * @brief Start a new game (reset and enter Playing state)
   */
  void startNewGame();

  /**
   * @brief Handle game over event
   * @param finalScore The player's final score
   */
  void onGameOver(int finalScore);

  /**
   * @brief Save current game to file
   */
  void saveGame();

  /**
   * @brief Load game from save file
   */
  void loadGame();

  /**
   * @brief Close the game window
   */
  void closeWindow();

  /**
   * @brief Get the render window
   * @return Pointer to the SFML render window
   */
  [[nodiscard]] sf::RenderWindow *getWindow() const noexcept {
    return window_.get();
  }

  /**
   * @brief Get the window size
   * @return Window dimensions as sf::Vector2u
   */
  [[nodiscard]] sf::Vector2u getWindowSize() const;

  /**
   * @brief Get the current font for word rendering
   * @return Reference to the currently selected font
   */
  [[nodiscard]] sf::Font &getCurrentFont();

  /**
   * @brief Get the name of the currently selected font
   * @return Font name string
   */
  [[nodiscard]] const std::string &getCurrentFontName() const noexcept {
    return currentFontName_;
  }

  /**
   * @brief Set the current font
   * @param fontName Name of the font to use
   */
  void setCurrentFont(const std::string &fontName);

private:
  void initializeWindow();
  void createState(StateType stateType);

  static constexpr int kDefaultWidth = 1280;
  static constexpr int kDefaultHeight = 720;
  static constexpr int kDefaultFPS = 60;

  std::unique_ptr<sf::RenderWindow> window_;
  std::unique_ptr<IGameState> currentState_;
  StateType currentStateType_ = StateType::Menu;

  ScoreManager scoreManager_;
  std::filesystem::path basePath_;
  std::string currentFontName_ = "DarlingCoffee";
  bool fontChosen_ = false;
};

#endif // MONKEY_TYPER_GAME_H
