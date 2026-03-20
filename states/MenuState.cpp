/**
 * @file MenuState.cpp
 * @brief Implementation of the main menu state
 * @date 2024
 */

#include "MenuState.h"
#include "../AudioManager.h"
#include "../Game.h"
#include "../ResourceManager.h"


MenuState::MenuState() = default;

void MenuState::enter(Game &game) { initializeUI(game.getWindowSize()); }

void MenuState::exit(Game &game) {
  // No cleanup needed
}

void MenuState::update(Game &game, float deltaTime) {
  // Menu is static, no update needed
}

void MenuState::render(Game &game, sf::RenderWindow &window) {
  auto &resources = ResourceManager::getInstance();
  auto &audio = AudioManager::getInstance();

  backgroundSprite_.setTexture(resources.getTexture("background"));
  window.draw(backgroundSprite_);
  window.draw(gameTitleText_);
  window.draw(fontSettingText_);
  window.draw(loadGameText_);
  window.draw(startGameText_);
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

void MenuState::handleEvent(Game &game, const sf::Event &event) {
  auto &audio = AudioManager::getInstance();

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Enter) {
      audio.playSound("approve");
      game.startNewGame();
    } else if (event.key.code == sf::Keyboard::C) {
      audio.playSound("approve");
      game.changeState(Game::StateType::FontSelect);
    } else if (event.key.code == sf::Keyboard::L) {
      audio.playSound("approve");
      game.loadGame();
    } else if (event.key.code == sf::Keyboard::Escape) {
      game.closeWindow();
    } else if (event.key.code == sf::Keyboard::F1) {
      audio.toggleMute();
    }
  }
}

void MenuState::initializeUI(const sf::Vector2u &windowSize) {
  auto &resources = ResourceManager::getInstance();
  auto &defaultFont = resources.getDefaultFont();
  auto windowWidth = static_cast<float>(windowSize.x);
  auto windowHeight = static_cast<float>(windowSize.y);

  // Game title
  gameTitleText_.setFont(defaultFont);
  gameTitleText_.setCharacterSize(100);
  gameTitleText_.setString("Monkey Typer");
  gameTitleText_.setFillColor(sf::Color(255, 181, 30, 150));
  gameTitleText_.setPosition(
      (windowWidth - gameTitleText_.getGlobalBounds().width) / 2.0f, 20.0f);

  // Font setting option
  fontSettingText_.setFont(defaultFont);
  fontSettingText_.setCharacterSize(50);
  fontSettingText_.setString("Press C to choose font");
  fontSettingText_.setFillColor(sf::Color(255, 255, 255, 100));
  fontSettingText_.setPosition(
      (windowWidth - fontSettingText_.getGlobalBounds().width) / 2.0f,
      windowHeight / 2.0f - 20.0f);

  // Load game option
  loadGameText_.setFont(defaultFont);
  loadGameText_.setCharacterSize(50);
  loadGameText_.setString("Press L to load game");
  loadGameText_.setFillColor(sf::Color(255, 255, 255, 100));
  loadGameText_.setPosition(
      (windowWidth - loadGameText_.getGlobalBounds().width) / 2.0f,
      windowHeight / 2.0f - 20.0f + fontSettingText_.getGlobalBounds().height);

  // Start game option
  startGameText_.setFont(defaultFont);
  startGameText_.setCharacterSize(60);
  startGameText_.setString("Press Enter to start");
  startGameText_.setFillColor(sf::Color(0, 0, 255, 125));
  startGameText_.setPosition(
      (windowWidth - startGameText_.getGlobalBounds().width) / 2.0f,
      windowHeight - startGameText_.getGlobalBounds().height - 20.0f);

  // ESC hint
  escText_.setFont(defaultFont);
  escText_.setCharacterSize(20);
  escText_.setString("Press ESC to quit");
  escText_.setFillColor(sf::Color(35, 165, 9, 150));
  escText_.setPosition(windowWidth * 0.86f, windowHeight * 0.96f);
}
