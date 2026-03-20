/**
 * @file AudioManager.cpp
 * @brief Implementation of the AudioManager singleton
 * @date 2024
 */

#include "AudioManager.h"
#include "ResourceManager.h"

AudioManager &AudioManager::getInstance() {
  static AudioManager instance;
  return instance;
}

void AudioManager::initialize() {
  auto &resources = ResourceManager::getInstance();

  const std::vector<std::string> soundNames = {"correct", "missed", "gameOver",
                                               "approve"};

  for (const auto &name : soundNames) {
    sounds_[name].setBuffer(resources.getSoundBuffer(name));
    sounds_[name].setVolume(volume_);
  }
}

void AudioManager::playSound(const std::string &name) {
  if (muted_) {
    return;
  }

  auto it = sounds_.find(name);
  if (it != sounds_.end()) {
    it->second.play();
  }
}

void AudioManager::toggleMute() {
  muted_ = !muted_;

  float effectiveVolume = muted_ ? 0.0f : volume_;
  for (auto &[name, sound] : sounds_) {
    sound.setVolume(effectiveVolume);
  }
}

bool AudioManager::isMuted() const noexcept { return muted_; }

void AudioManager::setVolume(float volume) {
  volume_ = volume;
  if (!muted_) {
    for (auto &[name, sound] : sounds_) {
      sound.setVolume(volume_);
    }
  }
}
