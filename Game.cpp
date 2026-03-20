/**
 * @file Game.cpp
 * @brief Implementation of the main Game controller
 * @date 2024
 */

#include "Game.h"
#include "AudioManager.h"
#include "ResourceManager.h"
#include "states/FontSelectState.h"
#include "states/GameOverState.h"
#include "states/MenuState.h"
#include "states/PlayingState.h"
#include <fstream>
#include <iostream>


Game::Game() {
  basePath_ = std::filesystem::absolute("..").string();
  initializeWindow();
}

void Game::initializeWindow() {
  // Read window configuration
  auto configPath = basePath_ / "Config" / "windowConfig.txt";
  std::string title = "Monkey Typer";
  int width = kDefaultWidth;
  int height = kDefaultHeight;
  int fps = kDefaultFPS;

  std::ifstream config(configPath);
  if (config.is_open()) {
    std::getline(config, title);
    config >> width >> height >> fps;
  }

  // Create window
  window_ = std::make_unique<sf::RenderWindow>(
      sf::VideoMode(width, height), title,
      sf::Style::Close | sf::Style::Titlebar);
  window_->setFramerateLimit(fps);

  // Load resources
  auto &resources = ResourceManager::getInstance();
  resources.loadResources(basePath_);

  // Initialize audio
  auto &audio = AudioManager::getInstance();
  audio.initialize();

  // Start with Menu state
  changeState(StateType::Menu);
}

void Game::run() {
  sf::Clock clock;

  while (window_->isOpen()) {
    float deltaTime = clock.restart().asSeconds();

    // Handle events
    sf::Event event;
    while (window_->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_->close();
      } else if (currentState_) {
        currentState_->handleEvent(*this, event);
      }
    }

    // Update
    if (currentState_) {
      currentState_->update(*this, deltaTime);
    }

    // Render
    window_->clear();
    if (currentState_) {
      currentState_->render(*this, *window_);
    }
    window_->display();
  }
}

void Game::changeState(StateType stateType) {
  if (currentState_) {
    currentState_->exit(*this);
  }

  currentStateType_ = stateType;
  createState(stateType);

  if (currentState_) {
    currentState_->enter(*this);
  }
}

void Game::createState(StateType stateType) {
  switch (stateType) {
  case StateType::Menu:
    currentState_ = std::make_unique<MenuState>();
    break;
  case StateType::Playing:
    currentState_ = std::make_unique<PlayingState>();
    break;
  case StateType::GameOver:
    currentState_ = std::make_unique<GameOverState>();
    break;
  case StateType::FontSelect:
    currentState_ = std::make_unique<FontSelectState>();
    break;
  }
}

void Game::startNewGame() {
  if (!fontChosen_) {
    currentFontName_ = "DarlingCoffee";
  }
  changeState(StateType::Playing);
}

void Game::onGameOver(int finalScore) {
  // Save score
  auto scoresPath = (basePath_ / "Config" / "topscores.txt").string();
  scoreManager_.addScore(finalScore, scoresPath);

  // Create and configure game over state
  auto gameOverState = std::make_unique<GameOverState>();

  if (currentState_) {
    currentState_->exit(*this);
  }

  currentStateType_ = StateType::GameOver;
  currentState_ = std::move(gameOverState);

  // Set score data
  auto *state = dynamic_cast<GameOverState *>(currentState_.get());
  if (state) {
    state->setFinalScore(finalScore);
    state->setTopScores(scoreManager_.getTopScores());
  }

  currentState_->enter(*this);
}

void Game::saveGame() {
  auto savePath = basePath_ / "Config" / "saveGame.txt";

  auto *playingState = dynamic_cast<PlayingState *>(currentState_.get());
  if (!playingState) {
    return;
  }

  std::ofstream file(savePath);
  if (file.is_open()) {
    file << playingState->getScore() << "\n";
    file << playingState->getMissed() << "\n";
    file << playingState->getWordSpeedMultiplier() << "\n";
    file << playingState->getGeneratingSpeedMultiplier() << "\n";

    const auto &words = playingState->getWords();
    file << words.size() << "\n";
    for (const auto &word : words) {
      file << word.getText() << " " << word.getTextObject().getPosition().x
           << " " << word.getTextObject().getPosition().y << "\n";
    }
  }

  changeState(StateType::Menu);
}

void Game::loadGame() {
  auto savePath = basePath_ / "Config" / "saveGame.txt";

  std::ifstream file(savePath);
  if (!file.is_open()) {
    return;
  }

  int score, missed;
  float wordSpeed, genSpeed;
  file >> score >> missed >> wordSpeed >> genSpeed;

  int wordCount;
  file >> wordCount;

  std::vector<std::tuple<std::string, float, float>> savedWords;
  for (int i = 0; i < wordCount; ++i) {
    std::string wordStr;
    float posX, posY;
    file >> wordStr >> posX >> posY;
    savedWords.emplace_back(wordStr, posX, posY);
  }

  // Start a new game and load saved data
  changeState(StateType::Playing);

  auto *playingState = dynamic_cast<PlayingState *>(currentState_.get());
  if (playingState) {
    playingState->loadFromSave(score, missed, wordSpeed, genSpeed, savedWords);
  }
}

void Game::closeWindow() {
  if (window_) {
    window_->close();
  }
}

sf::Vector2u Game::getWindowSize() const {
  if (window_) {
    return window_->getSize();
  }
  return {kDefaultWidth, kDefaultHeight};
}

sf::Font &Game::getCurrentFont() {
  auto &resources = ResourceManager::getInstance();
  return resources.getFont(currentFontName_);
}

void Game::setCurrentFont(const std::string &fontName) {
  currentFontName_ = fontName;
  fontChosen_ = true;
}
