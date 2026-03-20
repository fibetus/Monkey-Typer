/**
 * @file WordGenerator.cpp
 * @brief Implementation of the WordGenerator class
 * @date 2024
 */

#include "WordGenerator.h"
#include <fstream>
#include <stdexcept>

WordGenerator::WordGenerator(const std::filesystem::path &filename,
                             const sf::Font &font)
    : font_(&font), rng_(std::random_device{}()) {

  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Could not open words file: " + filename.string());
  }

  std::string word;
  while (file >> word) {
    words_.push_back(word);
  }

  if (words_.empty()) {
    throw std::runtime_error("No words loaded from file: " + filename.string());
  }
}

Word WordGenerator::generateWord(float speed, float posY) {
  if (words_.empty()) {
    throw std::runtime_error("No words loaded");
  }

  std::uniform_int_distribution<std::size_t> dist(0, words_.size() - 1);
  const std::string &selectedWord = words_[dist(rng_)];

  return Word(selectedWord, *font_, speed, posY);
}
