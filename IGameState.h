/**
 * @file IGameState.h
 * @brief Abstract interface for game state pattern implementation
 * @date 2024
 */

#ifndef MONKEY_TYPER_IGAMESTATE_H
#define MONKEY_TYPER_IGAMESTATE_H

#include <SFML/Graphics.hpp>

class Game;

/**
 * @brief Abstract base class for all game states
 * 
 * Implements the State Pattern to manage different screens/modes
 * of the game (Menu, Playing, GameOver, FontSelect).
 */
class IGameState {
public:
    /**
     * @brief Virtual destructor for proper cleanup
     */
    virtual ~IGameState() = default;

    /**
     * @brief Called when entering this state
     * @param game Reference to the main game controller
     */
    virtual void enter(Game& game) = 0;

    /**
     * @brief Called when exiting this state
     * @param game Reference to the main game controller
     */
    virtual void exit(Game& game) = 0;

    /**
     * @brief Update game logic for this state
     * @param game Reference to the main game controller
     * @param deltaTime Time elapsed since last frame in seconds
     */
    virtual void update(Game& game, float deltaTime) = 0;

    /**
     * @brief Render this state to the window
     * @param game Reference to the main game controller
     * @param window Reference to the SFML render window
     */
    virtual void render(Game& game, sf::RenderWindow& window) = 0;

    /**
     * @brief Handle input events for this state
     * @param game Reference to the main game controller
     * @param event The SFML event to handle
     */
    virtual void handleEvent(Game& game, const sf::Event& event) = 0;
};

#endif // MONKEY_TYPER_IGAMESTATE_H
