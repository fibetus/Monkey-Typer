/**
 * @file ScoreManager.h
 * @brief Score management and persistence
 * @date 2024
 */

#ifndef MONKEY_TYPER_SCOREMANAGER_H
#define MONKEY_TYPER_SCOREMANAGER_H

#include <filesystem>
#include <string>
#include <vector>


/**
 * @brief Manages game scores and high score persistence
 *
 * Handles saving and loading of top scores from a file,
 * maintaining a sorted list of the best scores.
 */
class ScoreManager {
public:
  /**
   * @brief Maximum number of top scores to track
   */
  static constexpr std::size_t kMaxScores = 5;

  /**
   * @brief Default constructor
   */
  ScoreManager() = default;

  /**
   * @brief Add a new score and update the top scores file
   * @param score The score to add
   * @param filename Path to the scores file
   */
  void addScore(int score, const std::filesystem::path &filename);

  /**
   * @brief Get the current top scores
   * @return Reference to vector of top scores (sorted descending)
   */
  [[nodiscard]] const std::vector<int> &getTopScores() const noexcept {
    return topScores_;
  }

private:
  std::vector<int> topScores_;
};

#endif // MONKEY_TYPER_SCOREMANAGER_H
