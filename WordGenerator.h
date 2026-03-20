/**
 * @file WordGenerator.h
 * @brief Word generator for creating random words
 * @date 2024
 */

#ifndef MONKEY_TYPER_WORDGENERATOR_H
#define MONKEY_TYPER_WORDGENERATOR_H

#include "Word.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <random>
#include <string>
#include <vector>


/**
 * @brief Generates random words from a word list file
 *
 * Loads words from a file and provides random word generation
 * for the typing game using proper C++ random number generation.
 */
class WordGenerator {
public:
  /**
   * @brief Construct a WordGenerator from a word list file
   * @param filename Path to the word list file
   * @param font Reference to the font for word rendering
   * @throws std::runtime_error if file cannot be opened
   */
  WordGenerator(const std::filesystem::path &filename, const sf::Font &font);

  /**
   * @brief Default destructor
   */
  ~WordGenerator() = default;

  /**
   * @brief Generate a random word
   * @param speed Horizontal movement speed
   * @param posY Vertical position
   * @return A new Word object
   * @throws std::runtime_error if no words are loaded
   */
  [[nodiscard]] Word generateWord(float speed, float posY);

  /**
   * @brief Set the font for generated words
   * @param font Reference to the new font
   */
  void setFont(const sf::Font &font) noexcept { font_ = &font; }

  /**
   * @brief Get the number of loaded words
   * @return Number of words in the word list
   */
  [[nodiscard]] std::size_t getWordCount() const noexcept {
    return words_.size();
  }

private:
  std::vector<std::string> words_;
  const sf::Font *font_;
  std::mt19937 rng_;
};

#endif // MONKEY_TYPER_WORDGENERATOR_H
