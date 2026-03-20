/**
 * @file Word.h
 * @brief Word entity for the typing game
 * @date 2024
 */

#ifndef MONKEY_TYPER_WORD_H
#define MONKEY_TYPER_WORD_H

#include <SFML/Graphics.hpp>
#include <string>
#include <string_view>

/**
 * @brief Represents a single word in the typing game
 *
 * Handles word display, movement, character matching, and color animation
 * based on position on screen.
 */
class Word {
public:
  /**
   * @brief Construct a new Word
   * @param text The word string to display
   * @param font Reference to the font to use
   * @param speed Initial horizontal movement speed
   * @param posY Vertical position on screen
   */
  Word(std::string_view text, const sf::Font &font, float speed, float posY);

  /**
   * @brief Default destructor
   */
  ~Word() = default;

  /**
   * @brief Update word position based on elapsed time
   * @param deltaTime Time since last frame in seconds
   */
  void update(float deltaTime);

  /**
   * @brief Draw the word to the render window
   * @param window Reference to the SFML render window
   */
  void draw(sf::RenderWindow &window);

  /**
   * @brief Check if word has moved off the right edge of screen
   * @param windowWidth Width of the game window
   * @return True if word is off screen
   */
  [[nodiscard]] bool isOffScreen(unsigned int windowWidth) const noexcept;

  /**
   * @brief Attempt to match an input character
   * @param input The character typed by the player
   */
  void matchCharacter(char input);

  /**
   * @brief Check if the entire word has been typed
   * @return True if all characters have been matched
   */
  [[nodiscard]] bool isComplete() const noexcept;

  /**
   * @brief Equality comparison operator
   * @param other Word to compare against
   * @return True if words are equal
   */
  bool operator==(const Word &other) const;

  /**
   * @brief Get the word string
   * @return Reference to the word string
   */
  [[nodiscard]] const std::string &getText() const noexcept { return text_; }

  /**
   * @brief Get the SFML text object
   * @return Reference to the sf::Text object
   */
  [[nodiscard]] const sf::Text &getTextObject() const noexcept {
    return textObject_;
  }

  /**
   * @brief Get the current word speed
   * @return Current horizontal speed
   */
  [[nodiscard]] float getWordSpeed() const noexcept { return speed_; }

  /**
   * @brief Set the word speed
   * @param speed New horizontal speed
   */
  void setWordSpeed(float speed) noexcept { speed_ = speed; }

  /**
   * @brief Set the word position
   * @param position New position as sf::Vector2f
   */
  void setPosition(const sf::Vector2f &position);

  /**
   * @brief Set the SFML text object
   * @param textObject New sf::Text object
   */
  void setTextObject(const sf::Text &textObject);

private:
  /**
   * @brief Apply color animation based on screen position
   * @param windowWidth Width of the game window
   */
  void applyColorAnimation(unsigned int windowWidth);

  static constexpr unsigned int kDefaultCharacterSize = 30;

  sf::Text textObject_;
  std::string text_;
  float speed_;
  std::size_t matchedCharIndex_ = 0;
};

#endif // MONKEY_TYPER_WORD_H
