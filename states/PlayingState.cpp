/**
 * @file PlayingState.cpp
 * @brief Implementation of the active gameplay state
 * @date 2024
 */

#include "PlayingState.h"
#include "../AudioManager.h"
#include "../Game.h"
#include "../ResourceManager.h"
#include <algorithm>


PlayingState::PlayingState() : rng_(std::random_device{}()) {}

void PlayingState::enter(Game &game) {
  auto &resources = ResourceManager::getInstance();
  auto windowSize = game.getWindowSize();
  auto &gameFont = game.getCurrentFont();

  // Initialize word generator
  wordGenerator_ =
      std::make_unique<WordGenerator>("../Config/1000words.txt", gameFont);

  // Calculate row count based on window height
  rowCount_ = static_cast<int>(static_cast<float>(windowSize.y) * 0.8f /
                               (gameFont.getLineSpacing(30) + 5));
  rowsOccupied_.assign(rowCount_, nullptr);

  // Initialize UI
  initializeBottomPanel(windowSize);

  // Spawn first word
  spawningClock_.restart();
  spawnWord(game);
}

void PlayingState::exit(Game &game) {
  words_.clear();
  rowsOccupied_.clear();
  currentInput_.clear();
}

void PlayingState::update(Game &game, float deltaTime) {
  auto &audio = AudioManager::getInstance();
  auto windowSize = game.getWindowSize();

  if (gameLoaded_) {
    gameLoaded_ = false;
    currentInput_.clear();
  }

  updateMultipliers();

  // Update all words
  float speed = kBaseWordSpeed * wordSpeedMultiplier_;
  for (auto &word : words_) {
    word.update(deltaTime);
    word.setWordSpeed(speed);
  }

  // Remove off-screen words
  auto it = std::remove_if(words_.begin(), words_.end(), [&](const Word &word) {
    if (word.isOffScreen(windowSize.x)) {
      audio.playSound("missed");
      missed_++;
      return true;
    }
    return false;
  });
  words_.erase(it, words_.end());

  // Update row occupancy
  updateRowOccupancy(*game.getWindow());

  // Check game over
  if (missed_ >= kMaxMissed) {
    audio.playSound("gameOver");
    game.onGameOver(score_);
    return;
  }

  // Spawn new words
  if (spawningClock_.getElapsedTime().asSeconds() >
      generatingSpeedMultiplier_) {
    spawnWord(game);
    spawningClock_.restart();
  }

  // Update bottom panel
  scoreDisplay_.setString("Score: " + std::to_string(score_));
  fontDisplay_.setString("Font: " + game.getCurrentFont().getInfo().family);
  inputDisplay_.setString("Input: " + currentInput_);
  missedWordsDisplay_.setString(std::to_string(missed_));
}

void PlayingState::render(Game &game, sf::RenderWindow &window) {
  auto &resources = ResourceManager::getInstance();
  auto &audio = AudioManager::getInstance();

  backgroundSprite_.setTexture(resources.getTexture("background"));
  window.draw(backgroundSprite_);
  window.draw(bottomPanel_);
  window.draw(scoreDisplay_);
  window.draw(fontDisplay_);
  window.draw(inputDisplay_);
  window.draw(missedWordsDisplay_);
  window.draw(escText_);

  if (audio.isMuted()) {
    window.draw(muteIconSprite_);
  }

  for (auto &word : words_) {
    word.draw(window);
  }
}

void PlayingState::handleEvent(Game &game, const sf::Event &event) {
  auto &audio = AudioManager::getInstance();

  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode < 128) {
      handleTextInput(game, static_cast<char>(event.text.unicode));
    }
  } else if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      game.closeWindow();
    } else if (event.key.code == sf::Keyboard::F1) {
      audio.toggleMute();
    } else if (event.key.control && event.key.code == sf::Keyboard::S) {
      audio.playSound("approve");
      game.saveGame();
    }
  }
}

void PlayingState::handleTextInput(Game &game, char input) {
  auto &audio = AudioManager::getInstance();

  if (input == '\b' && !currentInput_.empty()) {
    currentInput_.pop_back();
  } else if (input != '\b' && input != ' ') {
    currentInput_ += input;
  }

  if ((input == '\r' && !currentInput_.empty()) || currentInput_.size() > 15) {
    currentInput_.clear();
  }

  bool foundCompleteWord = false;
  for (auto &word : words_) {
    word.matchCharacter(input);
    if (word.isComplete() && word.getText() == currentInput_) {
      score_ += static_cast<int>(10 * wordSpeedMultiplier_ *
                                 (4 - generatingSpeedMultiplier_) *
                                 static_cast<float>(word.getText().size()));
      words_.erase(std::remove(words_.begin(), words_.end(), word),
                   words_.end());
      audio.playSound("correct");
      foundCompleteWord = true;
      break;
    }
  }

  if (foundCompleteWord) {
    currentInput_.clear();
  }
}

void PlayingState::spawnWord(Game &game) {
  auto &gameFont = game.getCurrentFont();
  wordGenerator_->setFont(gameFont);

  auto windowSize = game.getWindowSize();
  auto windowWidth = static_cast<float>(windowSize.x);
  constexpr int rowHeight = 32;

  std::vector<int> availableRows;
  for (int i = 0; i < rowCount_; ++i) {
    availableRows.push_back(i);
  }

  if (waitingForRow_) {
    if (waitClock_.getElapsedTime().asSeconds() <
        (2.0f - generatingSpeedMultiplier_)) {
      return;
    }
    waitingForRow_ = false;
    waitClock_.restart();
  }

  int rowIndex = -1;
  bool rowFound = false;

  while (!rowFound && !availableRows.empty()) {
    std::shuffle(availableRows.begin(), availableRows.end(), rng_);

    for (int currentRow : availableRows) {
      float rowSpaceUsed = 0.0f;
      float maxWordWidth = 0.0f;

      for (const auto &word : words_) {
        if (rowsOccupied_[currentRow] == &word) {
          auto wordWidth = static_cast<float>(word.getText().length()) *
                           gameFont.getLineSpacing(30);
          auto distanceTraveled = word.getTextObject().getPosition().x;
          auto distanceRemaining = windowWidth - (distanceTraveled + wordWidth);
          auto totalDistance = distanceTraveled + distanceRemaining *
                                                      word.getWordSpeed() /
                                                      kBaseWordSpeed;
          rowSpaceUsed += totalDistance;
          maxWordWidth = std::max(
              maxWordWidth, wordWidth * word.getWordSpeed() / kBaseWordSpeed);
        }
      }

      float spaceAvailable = windowWidth - rowSpaceUsed;
      if (rowsOccupied_[currentRow] == nullptr ||
          spaceAvailable > maxWordWidth + gameFont.getLineSpacing(30) + 10.0f) {
        rowIndex = currentRow;
        rowFound = true;
        break;
      }
    }
    availableRows.clear();
  }

  if (rowIndex == -1) {
    waitingForRow_ = true;
    waitClock_.restart();
    return;
  }

  auto posY = static_cast<float>(rowIndex * (rowHeight + 3));
  auto speed = kBaseWordSpeed * wordSpeedMultiplier_;

  words_.push_back(wordGenerator_->generateWord(speed, posY));

  if (!words_.empty()) {
    std::uniform_int_distribution<> distrib(0, static_cast<int>(words_.size()) -
                                                   1);
    rowsOccupied_[rowIndex] = &words_[distrib(rng_)];
  }
}

void PlayingState::updateMultipliers() {
  auto i = score_ / 100;
  auto j = score_ / 100;

  if (wordSpeedMultiplier_ < 2.6f) {
    wordSpeedMultiplier_ = 1.4f + static_cast<float>(i) / 25.0f;
    tempWordSpeedMultiplier_ = wordSpeedMultiplier_;
    tempScoreIndex_ = i;
  } else {
    wordSpeedMultiplier_ = tempWordSpeedMultiplier_ +
                           static_cast<float>(i - tempScoreIndex_) / 100.0f;
  }

  if (j < 70) {
    generatingSpeedMultiplier_ = 2.0f - static_cast<float>(j) / 50.0f;
  }
}

void PlayingState::updateRowOccupancy(const sf::RenderWindow &window) {
  auto windowWidth = static_cast<float>(window.getSize().x);

  for (int i = 0; i < rowCount_; ++i) {
    if (rowsOccupied_[i] != nullptr) {
      bool wordFound = false;
      for (const auto &word : words_) {
        if (rowsOccupied_[i] == &word) {
          wordFound = true;
          if (word.getTextObject().getPosition().x > windowWidth * 0.75f) {
            rowsOccupied_[i] = nullptr;
          }
          break;
        }
      }
      if (!wordFound) {
        rowsOccupied_[i] = nullptr;
      }
    }
  }
}

void PlayingState::initializeBottomPanel(const sf::Vector2u &windowSize) {
  auto &resources = ResourceManager::getInstance();
  auto &defaultFont = resources.getDefaultFont();
  auto windowWidth = static_cast<float>(windowSize.x);
  auto windowHeight = static_cast<float>(windowSize.y);

  // Bottom panel background
  bottomPanel_.setSize(sf::Vector2f(windowWidth, windowHeight * 0.2f));
  bottomPanel_.setPosition(0, windowHeight * 0.8f);
  bottomPanel_.setFillColor(sf::Color(235, 232, 226, 75));

  // Score display
  scoreDisplay_.setFont(defaultFont);
  scoreDisplay_.setCharacterSize(20);
  scoreDisplay_.setFillColor(sf::Color(255, 255, 255, 155));
  scoreDisplay_.setPosition(windowWidth * 0.01f, windowHeight * 0.82f);

  // Font display
  fontDisplay_.setFont(defaultFont);
  fontDisplay_.setCharacterSize(20);
  fontDisplay_.setFillColor(sf::Color(255, 255, 255, 155));
  fontDisplay_.setPosition(windowWidth * 0.01f, windowHeight * 0.86f);

  // Input display
  inputDisplay_.setFont(defaultFont);
  inputDisplay_.setCharacterSize(30);
  inputDisplay_.setFillColor(sf::Color(255, 255, 255, 155));
  auto center = (windowHeight * 0.90f - windowHeight * 0.80f -
                 static_cast<float>(inputDisplay_.getCharacterSize())) /
                2.0f;
  inputDisplay_.setPosition(windowWidth * 0.4f, windowHeight * 0.90f - center);

  // Missed words display
  missedWordsDisplay_.setFont(defaultFont);
  missedWordsDisplay_.setCharacterSize(60);
  missedWordsDisplay_.setFillColor(sf::Color(255, 0, 0, 175));
  missedWordsDisplay_.setString("0");
  missedWordsDisplay_.setPosition(
      windowWidth - missedWordsDisplay_.getGlobalBounds().width - 10.0f,
      windowHeight * 0.85f - center);

  // ESC hint
  escText_.setFont(defaultFont);
  escText_.setCharacterSize(20);
  escText_.setString("Press ESC to quit");
  escText_.setFillColor(sf::Color(35, 165, 9, 150));
  escText_.setPosition(windowWidth * 0.86f, windowHeight * 0.96f);

  // Mute icon
  muteIconSprite_.setTexture(resources.getTexture("muteIcon"));
  auto scaleWidth =
      50.0f / static_cast<float>(resources.getTexture("muteIcon").getSize().x);
  auto scaleHeight =
      50.0f / static_cast<float>(resources.getTexture("muteIcon").getSize().y);
  muteIconSprite_.setScale(scaleWidth, scaleHeight);
  muteIconSprite_.setColor(sf::Color(255, 0, 0, 150));
  muteIconSprite_.setPosition(
      10.0f, windowHeight - muteIconSprite_.getGlobalBounds().height - 10.0f);
}

void PlayingState::loadFromSave(
    int score, int missed, float wordSpeed, float genSpeed,
    const std::vector<std::tuple<std::string, float, float>> &savedWords) {
  score_ = score;
  missed_ = missed;
  wordSpeedMultiplier_ = wordSpeed;
  generatingSpeedMultiplier_ = genSpeed;
  gameLoaded_ = true;

  words_.clear();
  // Words will be recreated by Game::loadGame
}
