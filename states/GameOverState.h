/**
 * @file GameOverState.h
 * @brief Game over state implementation
 * @date 2024
 */

#ifndef MONKEY_TYPER_GAMEOVERSTATE_H
#define MONKEY_TYPER_GAMEOVERSTATE_H

#include "../IGameState.h"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Game over state
 *
 * Displays the final score, top scores, and options to replay or return to
 * menu.
 */
class GameOverState : public IGameState {
public:
  GameOverState();
  ~GameOverState() override = default;

  void enter(Game &game) override;
  void exit(Game &game) override;
  void update(Game &game, float deltaTime) override;
  void render(Game &game, sf::RenderWindow &window) override;
  void handleEvent(Game &game, const sf::Event &event) override;

  /**
   * @brief Set the final score to display
   * @param score Final player score
   */
  void setFinalScore(int score);

  /**
   * @brief Set top scores to display
   * @param scores Vector of top scores
   */
  void setTopScores(const std::vector<int> &scores);

private:
  void initializeUI(const sf::Vector2u &windowSize);

  int finalScore_ = 0;
  std::vector<int> topScores_;

  sf::Text gameOverText_;
  sf::Text finalScoreText_;
  sf::Text yourScoreText_;
  sf::Text playAgainText_;
  sf::Text backToMenuText_;
  sf::Text topScoresText_;
  sf::Text escText_;
  sf::Sprite backgroundSprite_;
  sf::Sprite muteIconSprite_;
};

#endif // MONKEY_TYPER_GAMEOVERSTATE_H
