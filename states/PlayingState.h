/**
 * @file PlayingState.h
 * @brief Active gameplay state implementation
 * @date 2024
 */

#ifndef MONKEY_TYPER_PLAYINGSTATE_H
#define MONKEY_TYPER_PLAYINGSTATE_H

#include "../IGameState.h"
#include "../Word.h"
#include "../WordGenerator.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>


/**
 * @brief Active gameplay state
 *
 * Handles word spawning, player input, scoring, and game over detection.
 */
class PlayingState : public IGameState {
public:
  PlayingState();
  ~PlayingState() override = default;

  void enter(Game &game) override;
  void exit(Game &game) override;
  void update(Game &game, float deltaTime) override;
  void render(Game &game, sf::RenderWindow &window) override;
  void handleEvent(Game &game, const sf::Event &event) override;

  /**
   * @brief Get current score
   * @return Current player score
   */
  [[nodiscard]] int getScore() const noexcept { return score_; }

  /**
   * @brief Get number of missed words
   * @return Number of missed words
   */
  [[nodiscard]] int getMissed() const noexcept { return missed_; }

  /**
   * @brief Load game state from saved data
   * @param score Previous score
   * @param missed Previous missed count
   * @param wordSpeed Previous speed multiplier
   * @param genSpeed Previous generation speed multiplier
   * @param savedWords Vector of saved word data (string, x, y)
   */
  void loadFromSave(
      int score, int missed, float wordSpeed, float genSpeed,
      const std::vector<std::tuple<std::string, float, float>> &savedWords);

  /**
   * @brief Get current word speed multiplier
   */
  [[nodiscard]] float getWordSpeedMultiplier() const noexcept {
    return wordSpeedMultiplier_;
  }

  /**
   * @brief Get current generation speed multiplier
   */
  [[nodiscard]] float getGeneratingSpeedMultiplier() const noexcept {
    return generatingSpeedMultiplier_;
  }

  /**
   * @brief Get all active words
   */
  [[nodiscard]] const std::vector<Word> &getWords() const noexcept {
    return words_;
  }

private:
  static constexpr int kMaxMissed = 5;
  static constexpr float kBaseWordSpeed = 50.0f;

  void handleTextInput(Game &game, char input);
  void spawnWord(Game &game);
  void updateMultipliers();
  void updateRowOccupancy(const sf::RenderWindow &window);
  void initializeBottomPanel(const sf::Vector2u &windowSize);

  // Game state
  int score_ = 0;
  int missed_ = 0;
  float wordSpeedMultiplier_ = 1.0f;
  float generatingSpeedMultiplier_ = 2.0f;
  float tempWordSpeedMultiplier_ = 0.0f;
  int tempScoreIndex_ = 0;
  bool gameLoaded_ = false;

  // Words
  std::vector<Word> words_;
  std::unique_ptr<WordGenerator> wordGenerator_;
  std::vector<Word *> rowsOccupied_;
  int rowCount_ = 0;
  bool waitingForRow_ = false;
  sf::Clock waitClock_;
  sf::Clock spawningClock_;

  // Random number generator
  std::mt19937 rng_;

  // Input
  std::string currentInput_;

  // UI elements
  sf::RectangleShape bottomPanel_;
  sf::Text scoreDisplay_;
  sf::Text fontDisplay_;
  sf::Text inputDisplay_;
  sf::Text missedWordsDisplay_;
  sf::Text escText_;
  sf::Sprite backgroundSprite_;
  sf::Sprite muteIconSprite_;
};

#endif // MONKEY_TYPER_PLAYINGSTATE_H
