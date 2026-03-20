/**
 * @file ResourceManager.cpp
 * @brief Implementation of the ResourceManager singleton
 * @date 2024
 */

#include "ResourceManager.h"
#include <iostream>

ResourceManager &ResourceManager::getInstance() {
  static ResourceManager instance;
  return instance;
}

void ResourceManager::loadResources(const std::filesystem::path &basePath) {
  // Load fonts
  const std::vector<std::pair<std::string, std::string>> fontFiles = {
      {"DarlingCoffee", "Font/DarlingCoffee.ttf"},
      {"PlayStory", "Font/PlayStory.otf"},
      {"Espressonal", "Font/Espressonal.otf"},
      {"Arial", "Font/Arial.ttf"}};

  for (const auto &[name, path] : fontFiles) {
    auto fullPath = basePath / path;
    if (!fonts_[name].loadFromFile(fullPath.string())) {
      throw std::runtime_error("Could not load font: " + fullPath.string());
    }
  }

  // Load textures
  const std::vector<std::pair<std::string, std::string>> textureFiles = {
      {"background", "Textures/monkeyShadowReversed.jpg"},
      {"muteIcon", "Textures/muteIcon.jpg"}};

  for (const auto &[name, path] : textureFiles) {
    auto fullPath = basePath / path;
    if (!textures_[name].loadFromFile(fullPath.string())) {
      std::cerr << "Warning: Could not load texture: " << fullPath.string()
                << std::endl;
    }
  }

  // Load sounds
  const std::vector<std::pair<std::string, std::string>> soundFiles = {
      {"correct", "Sounds/correctSound.mp3"},
      {"missed", "Sounds/missedSound.mp3"},
      {"gameOver", "Sounds/gameOverSound.mp3"},
      {"approve", "Sounds/approveSound.mp3"}};

  for (const auto &[name, path] : soundFiles) {
    auto fullPath = basePath / path;
    if (!soundBuffers_[name].loadFromFile(fullPath.string())) {
      throw std::runtime_error("Could not load sound: " + fullPath.string());
    }
  }
}

sf::Font &ResourceManager::getFont(const std::string &name) {
  auto it = fonts_.find(name);
  if (it == fonts_.end()) {
    throw std::runtime_error("Font not found: " + name);
  }
  return it->second;
}

sf::Texture &ResourceManager::getTexture(const std::string &name) {
  auto it = textures_.find(name);
  if (it == textures_.end()) {
    throw std::runtime_error("Texture not found: " + name);
  }
  return it->second;
}

sf::SoundBuffer &ResourceManager::getSoundBuffer(const std::string &name) {
  auto it = soundBuffers_.find(name);
  if (it == soundBuffers_.end()) {
    throw std::runtime_error("Sound buffer not found: " + name);
  }
  return it->second;
}

sf::Font &ResourceManager::getDefaultFont() {
  return getFont(defaultFontName_);
}
