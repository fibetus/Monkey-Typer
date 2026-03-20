/**
 * @file ResourceManager.h
 * @brief Singleton manager for game resources (fonts, textures, sounds)
 * @date 2024
 */

#ifndef MONKEY_TYPER_RESOURCEMANAGER_H
#define MONKEY_TYPER_RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <memory>

/**
 * @brief Singleton class managing all game resources
 * 
 * Centralizes loading and caching of fonts, textures, and sound buffers.
 * Resources are loaded on first access and cached for subsequent use.
 */
class ResourceManager {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the ResourceManager instance
     */
    static ResourceManager& getInstance();

    // Delete copy/move operations for singleton
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    /**
     * @brief Load all required resources from the file system
     * @param basePath Base path to the resource directories
     * @throws std::runtime_error if a required resource cannot be loaded
     */
    void loadResources(const std::filesystem::path& basePath);

    /**
     * @brief Get a font by name
     * @param name Font identifier (e.g., "DarlingCoffee", "Arial")
     * @return Reference to the loaded font
     * @throws std::runtime_error if font not found
     */
    [[nodiscard]] sf::Font& getFont(const std::string& name);

    /**
     * @brief Get a texture by name
     * @param name Texture identifier
     * @return Reference to the loaded texture
     * @throws std::runtime_error if texture not found
     */
    [[nodiscard]] sf::Texture& getTexture(const std::string& name);

    /**
     * @brief Get a sound buffer by name
     * @param name Sound identifier
     * @return Reference to the loaded sound buffer
     * @throws std::runtime_error if sound not found
     */
    [[nodiscard]] sf::SoundBuffer& getSoundBuffer(const std::string& name);

    /**
     * @brief Get the default font
     * @return Reference to the default (DarlingCoffee) font
     */
    [[nodiscard]] sf::Font& getDefaultFont();

private:
    ResourceManager() = default;

    std::unordered_map<std::string, sf::Font> fonts_;
    std::unordered_map<std::string, sf::Texture> textures_;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers_;
    std::string defaultFontName_ = "DarlingCoffee";
};

#endif // MONKEY_TYPER_RESOURCEMANAGER_H
