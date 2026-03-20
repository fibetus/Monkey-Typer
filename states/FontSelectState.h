/**
 * @file FontSelectState.h
 * @brief Font selection state implementation
 * @date 2024
 */

#ifndef MONKEY_TYPER_FONTSELECTSTATE_H
#define MONKEY_TYPER_FONTSELECTSTATE_H

#include "../IGameState.h"
#include <SFML/Graphics.hpp>
#include <array>

/**
 * @brief Font selection state
 *
 * Allows the player to choose their preferred font for the game.
 */
class FontSelectState : public IGameState {
public:
  FontSelectState();
  ~FontSelectState() override = default;

  void enter(Game &game) override;
  void exit(Game &game) override;
  void update(Game &game, float deltaTime) override;
  void render(Game &game, sf::RenderWindow &window) override;
  void handleEvent(Game &game, const sf::Event &event) override;

private:
  /**
   * @brief Update visual highlighting for selected font
   * @param selectedIndex Index of the newly selected font (0-3)
   */
  void updateSelection(int selectedIndex);
  void initializeUI(const sf::Vector2u &windowSize);

  static constexpr int kFontCount = 4;
  inline static const sf::Color kSelectedColor{255, 222, 0, 150};
  inline static const sf::Color kUnselectedColor{255, 255, 255, 150};

  int selectedFontIndex_ = 0;

  sf::Text titleText_;
  std::array<sf::Text, kFontCount> fontOptionTexts_;
  sf::Text confirmText_;
  sf::Text escText_;
  sf::Sprite backgroundSprite_;
  sf::Sprite muteIconSprite_;

  std::array<std::string, kFontCount> fontNames_ = {
      "DarlingCoffee", "PlayStory", "Espressonal", "Arial"};
};

#endif // MONKEY_TYPER_FONTSELECTSTATE_H
