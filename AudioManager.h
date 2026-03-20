/**
 * @file AudioManager.h
 * @brief Singleton manager for audio playback and mute control
 * @date 2024
 */

#ifndef MONKEY_TYPER_AUDIOMANAGER_H
#define MONKEY_TYPER_AUDIOMANAGER_H

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>


/**
 * @brief Singleton class managing audio playback
 *
 * Provides centralized control for playing sounds and managing
 * the global mute state.
 */
class AudioManager {
public:
  /**
   * @brief Get the singleton instance
   * @return Reference to the AudioManager instance
   */
  static AudioManager &getInstance();

  // Delete copy/move operations for singleton
  AudioManager(const AudioManager &) = delete;
  AudioManager &operator=(const AudioManager &) = delete;
  AudioManager(AudioManager &&) = delete;
  AudioManager &operator=(AudioManager &&) = delete;

  /**
   * @brief Initialize audio system with sound buffers
   */
  void initialize();

  /**
   * @brief Play a sound by name
   * @param name Sound identifier (e.g., "correct", "missed", "gameOver",
   * "approve")
   */
  void playSound(const std::string &name);

  /**
   * @brief Toggle mute state on/off
   */
  void toggleMute();

  /**
   * @brief Check if audio is muted
   * @return True if muted, false otherwise
   */
  [[nodiscard]] bool isMuted() const noexcept;

  /**
   * @brief Set the global volume level
   * @param volume Volume level (0-100)
   */
  void setVolume(float volume);

private:
  AudioManager() = default;

  static constexpr float kDefaultVolume = 20.0f;

  std::unordered_map<std::string, sf::Sound> sounds_;
  bool muted_ = false;
  float volume_ = kDefaultVolume;
};

#endif // MONKEY_TYPER_AUDIOMANAGER_H
