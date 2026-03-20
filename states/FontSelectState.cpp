/**
 * @file FontSelectState.cpp
 * @brief Implementation of the font selection state
 * @date 2024
 */

#include "FontSelectState.h"
#include "../AudioManager.h"
#include "../Game.h"
#include "../ResourceManager.h"


FontSelectState::FontSelectState() = default;

void FontSelectState::enter(Game &game) {
  initializeUI(game.getWindowSize());

  // Highlight current font selection
  for (size_t i = 0; i < fontNames_.size(); ++i) {
    if (fontNames_[i] == game.getCurrentFontName()) {
      selectedFontIndex_ = static_cast<int>(i);
      updateSelection(selectedFontIndex_);
      break;
    }
  }
}

void FontSelectState::exit(Game &game) {
  // No cleanup needed
}

void FontSelectState::update(Game &game, float deltaTime) {
  // Font selection is static
}

void FontSelectState::render(Game &game, sf::RenderWindow &window) {
  auto &resources = ResourceManager::getInstance();
  auto &audio = AudioManager::getInstance();

  backgroundSprite_.setTexture(resources.getTexture("background"));
  window.draw(backgroundSprite_);
  window.draw(titleText_);

  for (const auto &text : fontOptionTexts_) {
    window.draw(text);
  }

  window.draw(confirmText_);
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

void FontSelectState::handleEvent(Game &game, const sf::Event &event) {
  auto &audio = AudioManager::getInstance();

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Num1) {
      audio.playSound("approve");
      selectedFontIndex_ = 0;
      updateSelection(selectedFontIndex_);
      game.setCurrentFont(fontNames_[0]);
    } else if (event.key.code == sf::Keyboard::Num2) {
      audio.playSound("approve");
      selectedFontIndex_ = 1;
      updateSelection(selectedFontIndex_);
      game.setCurrentFont(fontNames_[1]);
    } else if (event.key.code == sf::Keyboard::Num3) {
      audio.playSound("approve");
      selectedFontIndex_ = 2;
      updateSelection(selectedFontIndex_);
      game.setCurrentFont(fontNames_[2]);
    } else if (event.key.code == sf::Keyboard::Num4) {
      audio.playSound("approve");
      selectedFontIndex_ = 3;
      updateSelection(selectedFontIndex_);
      game.setCurrentFont(fontNames_[3]);
    } else if (event.key.code == sf::Keyboard::Enter) {
      audio.playSound("approve");
      game.changeState(Game::StateType::Menu);
    } else if (event.key.code == sf::Keyboard::Escape) {
      game.closeWindow();
    } else if (event.key.code == sf::Keyboard::F1) {
      audio.toggleMute();
    }
  }
}

void FontSelectState::updateSelection(int selectedIndex) {
  for (int i = 0; i < kFontCount; ++i) {
    if (i == selectedIndex) {
      fontOptionTexts_[i].setFillColor(kSelectedColor);
    } else {
      fontOptionTexts_[i].setFillColor(kUnselectedColor);
    }
  }
}

void FontSelectState::initializeUI(const sf::Vector2u &windowSize) {
  auto &resources = ResourceManager::getInstance();
  auto &defaultFont = resources.getDefaultFont();
  auto windowWidth = static_cast<float>(windowSize.x);
  auto windowHeight = static_cast<float>(windowSize.y);
  auto menuX = windowWidth * 0.25f;

  // Title
  titleText_.setFont(defaultFont);
  titleText_.setCharacterSize(80);
  titleText_.setString("Font Chooser");
  titleText_.setFillColor(sf::Color(127, 126, 122, 150));
  titleText_.setPosition(
      (windowWidth - titleText_.getGlobalBounds().width) / 2.0f, 10.0f);

  // Font options
  const std::array<std::pair<std::string, std::string>, kFontCount> fontLabels =
      {{{"1", "DarlingCoffee"},
        {"2", "PlayStory"},
        {"3", "Espressonal"},
        {"4", "Arial"}}};

  for (int i = 0; i < kFontCount; ++i) {
    auto &font = resources.getFont(fontLabels[i].second);
    fontOptionTexts_[i].setFont(font);
    fontOptionTexts_[i].setCharacterSize(40);
    fontOptionTexts_[i].setString("Press " + fontLabels[i].first +
                                  " to choose " + font.getInfo().family);
    fontOptionTexts_[i].setFillColor(i == 0 ? kSelectedColor
                                            : kUnselectedColor);
    fontOptionTexts_[i].setPosition(menuX, windowHeight * (0.3f + 0.1f * i));
  }

  // Confirm hint
  confirmText_.setFont(defaultFont);
  confirmText_.setCharacterSize(20);
  confirmText_.setString("Press Enter to confirm");
  confirmText_.setFillColor(sf::Color(255, 0, 0, 100));
  confirmText_.setPosition(
      5.0f, windowHeight - confirmText_.getGlobalBounds().height - 5.0f);

  // ESC hint
  escText_.setFont(defaultFont);
  escText_.setCharacterSize(20);
  escText_.setString("Press ESC to quit");
  escText_.setFillColor(sf::Color(35, 165, 9, 150));
  escText_.setPosition(windowWidth * 0.86f, windowHeight * 0.96f);
}
