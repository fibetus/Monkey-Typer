/**
 * @file GameOverState.cpp
 * @brief Implementation of the game over state
 * @date 2024
 */

#include "GameOverState.h"
#include "../AudioManager.h"
#include "../Game.h"
#include "../ResourceManager.h"


GameOverState::GameOverState() = default;

void GameOverState::enter(Game &game) { initializeUI(game.getWindowSize()); }

void GameOverState::exit(Game &game) {
  // No cleanup needed
}

void GameOverState::update(Game &game, float deltaTime) {
  // Game over screen is static
}

void GameOverState::render(Game &game, sf::RenderWindow &window) {
  auto &resources = ResourceManager::getInstance();
  auto &audio = AudioManager::getInstance();

  backgroundSprite_.setTexture(resources.getTexture("background"));
  window.draw(backgroundSprite_);
  window.draw(gameOverText_);
  window.draw(finalScoreText_);
  window.draw(yourScoreText_);
  window.draw(playAgainText_);
  window.draw(backToMenuText_);
  window.draw(topScoresText_);
  window.draw(escText_);

  if (audio.isMuted()) {
    muteIconSprite_.setTexture(resources.getTexture("muteIcon"));
    auto scaleWidth = 50.0f / static_cast<float>(
                                  resources.getTexture("muteIcon").getSize().x);
    auto scaleHeight =
        50.0f /
        static_cast<float>(resources.getTexture("muteIcon").getSize().y);
    muteIconSprite_.setScale(scaleWidth, scaleHeight);
    muteIconSprite_.setColor(sf::Color(255, 0, 0, 150));
    muteIconSprite_.setPosition(static_cast<float>(window.getSize().x) -
                                    muteIconSprite_.getGlobalBounds().width -
                                    10.0f,
                                10.0f);
    window.draw(muteIconSprite_);
  }
}

void GameOverState::handleEvent(Game &game, const sf::Event &event) {
  auto &audio = AudioManager::getInstance();

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Enter) {
      audio.playSound("approve");
      game.startNewGame();
    } else if (event.key.code == sf::Keyboard::M) {
      audio.playSound("approve");
      game.changeState(Game::StateType::Menu);
    } else if (event.key.code == sf::Keyboard::Escape) {
      game.closeWindow();
    } else if (event.key.code == sf::Keyboard::F1) {
      audio.toggleMute();
    }
  }
}

void GameOverState::setFinalScore(int score) {
  finalScore_ = score;
  yourScoreText_.setString(std::to_string(score));
}

void GameOverState::setTopScores(const std::vector<int> &scores) {
  topScores_ = scores;

  std::string scoreStr = "Top scores:\n";
  for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(5)); ++i) {
    scoreStr += std::to_string(scores[i]) + "\n";
  }
  topScoresText_.setString(scoreStr);
}

void GameOverState::initializeUI(const sf::Vector2u &windowSize) {
  auto &resources = ResourceManager::getInstance();
  auto &defaultFont = resources.getDefaultFont();
  auto windowWidth = static_cast<float>(windowSize.x);
  auto windowHeight = static_cast<float>(windowSize.y);

  // Game Over title
  gameOverText_.setFont(defaultFont);
  gameOverText_.setCharacterSize(80);
  gameOverText_.setFillColor(sf::Color(255, 0, 0, 125));
  gameOverText_.setString("Game Over!");
  gameOverText_.setPosition(
      windowWidth / 2.0f - gameOverText_.getGlobalBounds().width / 2.0f,
      windowHeight / 4.0f - gameOverText_.getGlobalBounds().height / 2.0f);

  // "Your Score" label
  finalScoreText_.setFont(defaultFont);
  finalScoreText_.setCharacterSize(60);
  finalScoreText_.setFillColor(sf::Color(0, 255, 0, 125));
  finalScoreText_.setString("Your Score:");
  finalScoreText_.setPosition(
      windowWidth / 2.0f - finalScoreText_.getGlobalBounds().width / 2.0f,
      windowHeight / 2.0f - finalScoreText_.getGlobalBounds().height / 2.0f);

  // Score value
  yourScoreText_.setFont(defaultFont);
  yourScoreText_.setCharacterSize(60);
  yourScoreText_.setFillColor(sf::Color(255, 255, 255, 125));
  yourScoreText_.setString(std::to_string(finalScore_));
  yourScoreText_.setPosition(
      windowWidth / 2.0f - yourScoreText_.getGlobalBounds().width / 2.0f,
      windowHeight / 2.0f + finalScoreText_.getGlobalBounds().height);

  // Play again hint
  playAgainText_.setFont(defaultFont);
  playAgainText_.setCharacterSize(30);
  playAgainText_.setFillColor(sf::Color(0, 0, 255, 125));
  playAgainText_.setString("Press Enter to Play Again");
  playAgainText_.setPosition(
      windowWidth / 2.0f - playAgainText_.getGlobalBounds().width / 2.0f,
      windowHeight - playAgainText_.getGlobalBounds().height - 10.0f);

  // Back to menu hint
  backToMenuText_.setFont(defaultFont);
  backToMenuText_.setCharacterSize(20);
  backToMenuText_.setString("Press M to go to Menu");
  backToMenuText_.setFillColor(sf::Color(255, 0, 0, 150));
  backToMenuText_.setPosition(
      5.0f, windowHeight - backToMenuText_.getGlobalBounds().height - 5.0f);

  // Top scores
  topScoresText_.setFont(defaultFont);
  topScoresText_.setCharacterSize(30);
  topScoresText_.setFillColor(sf::Color(125, 125, 125, 125));
  topScoresText_.setPosition(15.0f, 15.0f);

  // ESC hint
  escText_.setFont(defaultFont);
  escText_.setCharacterSize(20);
  escText_.setString("Press ESC to quit");
  escText_.setFillColor(sf::Color(35, 165, 9, 150));
  escText_.setPosition(windowWidth * 0.86f, windowHeight * 0.96f);
}
