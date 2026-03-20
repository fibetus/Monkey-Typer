/**
 * @file MenuState.h
 * @brief Main menu state implementation
 * @date 2024
 */

#ifndef MONKEY_TYPER_MENUSTATE_H
#define MONKEY_TYPER_MENUSTATE_H

#include "../IGameState.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Main menu state
 *
 * Displays the game title and options to start game, choose font,
 * or load a saved game.
 */
class MenuState : public IGameState {
public:
  MenuState();
  ~MenuState() override = default;

  void enter(Game &game) override;
  void exit(Game &game) override;
  void update(Game &game, float deltaTime) override;
  void render(Game &game, sf::RenderWindow &window) override;
  void handleEvent(Game &game, const sf::Event &event) override;

private:
  void initializeUI(const sf::Vector2u &windowSize);

  sf::Text gameTitleText_;
  sf::Text fontSettingText_;
  sf::Text loadGameText_;
  sf::Text startGameText_;
  sf::Text escText_;
  sf::Sprite backgroundSprite_;
  sf::Sprite muteIconSprite_;
};

#endif // MONKEY_TYPER_MENUSTATE_H
