/**
 * @file ScoreManager.cpp
 * @brief Implementation of the ScoreManager class
 * @date 2024
 */

#include "ScoreManager.h"
#include <algorithm>
#include <fstream>


void ScoreManager::addScore(int score, const std::filesystem::path &filename) {
  topScores_.clear();

  // Load existing scores
  std::ifstream inFile(filename);
  if (inFile.is_open()) {
    int tempScore;
    while (inFile >> tempScore) {
      topScores_.push_back(tempScore);
    }
  }

  // Add new score
  topScores_.push_back(score);

  // Sort descending
  std::sort(topScores_.begin(), topScores_.end(), std::greater<int>());

  // Keep only top scores
  if (topScores_.size() > kMaxScores) {
    topScores_.resize(kMaxScores);
  }

  // Save to file
  std::ofstream outFile(filename);
  if (outFile.is_open()) {
    for (int s : topScores_) {
      outFile << s << " ";
    }
  }
}
