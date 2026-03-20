/**
 * @file Word.cpp
 * @brief Implementation of the Word class
 * @date 2024
 */

#include "Word.h"

Word::Word(std::string_view text, const sf::Font &font, float speed, float posY)
    : text_(text), speed_(speed) {
  textObject_.setFont(font);
  textObject_.setString(sf::String(text_));
  textObject_.setCharacterSize(kDefaultCharacterSize);
  textObject_.setFillColor(sf::Color::White);
  textObject_.setPosition(0.0f, posY);
}

void Word::update(float deltaTime) {
  textObject_.move(speed_ * deltaTime, 0.0f);
}

void Word::draw(sf::RenderWindow &window) {
  applyColorAnimation(window.getSize().x);
  window.draw(textObject_);
}

bool Word::isOffScreen(unsigned int windowWidth) const noexcept {
  return textObject_.getPosition().x > static_cast<float>(windowWidth);
}

void Word::matchCharacter(char input) {
  if (matchedCharIndex_ < text_.size() && text_[matchedCharIndex_] == input) {
    ++matchedCharIndex_;
  }
}

bool Word::isComplete() const noexcept {
  return matchedCharIndex_ == text_.size();
}

bool Word::operator==(const Word &other) const {
  return text_ == other.text_ &&
         textObject_.getPosition() == other.textObject_.getPosition() &&
         speed_ == other.speed_;
}

void Word::setPosition(const sf::Vector2f &position) {
  textObject_.setPosition(position);
}

void Word::setTextObject(const sf::Text &textObject) {
  textObject_ = textObject;
}

void Word::applyColorAnimation(unsigned int windowWidth) {
  auto xPos = textObject_.getPosition().x;
  auto width = static_cast<float>(windowWidth);
  auto halfWidth = width * 0.5f;

  if (xPos > halfWidth) {
    auto relativePosition = (xPos - halfWidth) / halfWidth;

    if (relativePosition <= 0.33f) {
      // White to Yellow transition
      auto factor = relativePosition / 0.33f;
      auto blue = static_cast<sf::Uint8>(255.0f * (1.0f - factor));
      textObject_.setFillColor(sf::Color(255, 255, blue));
    } else if (relativePosition <= 0.66f) {
      // Yellow to Orange transition
      auto factor = (relativePosition - 0.33f) / 0.33f;
      auto green = static_cast<sf::Uint8>(255.0f * (1.0f - factor * 0.5f));
      textObject_.setFillColor(sf::Color(255, green, 0));
    } else {
      // Orange to Red transition
      auto factor = (relativePosition - 0.66f) / 0.34f;
      auto green = static_cast<sf::Uint8>(127.0f * (1.0f - factor));
      textObject_.setFillColor(sf::Color(255, green, 0));
    }
  } else {
    textObject_.setFillColor(sf::Color::White);
  }
}
