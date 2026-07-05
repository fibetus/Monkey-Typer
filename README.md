# Monkey Typer 🐒⌨️

A fast-paced typing game built with C++20 and SFML where words fall across the screen and you must type them before they escape!

## Features

- **Progressive Difficulty** — Words speed up as your score increases
- **Multiple Fonts** — Choose from 4 different font styles
- **High Score Tracking** — Top 5 scores saved between sessions
- **Save/Load** — Save your game mid-session with `Ctrl+S`
- **Sound Effects** — Audio feedback for correct/missed words
- **Visual Feedback** — Words change color as they approach the edge

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                           Game                                  │
│  - Manages window lifecycle and state transitions               │
│  - Uses State Pattern for clean screen management               │
└─────────────────────────────────────────────────────────────────┘
                              │
          ┌───────────────────┼───────────────────┐
          ▼                   ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│  IGameState     │ │ResourceManager  │ │  AudioManager   │
│  (Interface)    │ │  (Singleton)    │ │  (Singleton)    │
└─────────────────┘ │  - Fonts        │ │  - Sound playback│
          │         │  - Textures     │ │  - Mute control │
          │         │  - Sounds       │ └─────────────────┘
          ▼         └─────────────────┘
┌─────────────────────────────────────────────────────────────────┐
│              Concrete State Classes                             │
├─────────────────┬─────────────────┬─────────────────┬───────────┤
│   MenuState     │  PlayingState   │  GameOverState  │ FontSelect│
│   - Title       │  - Word spawning│  - Final score  │ - Font UI │
│   - Navigation  │  - Scoring      │  - Top scores   │ - Preview │
└─────────────────┴─────────────────┴─────────────────┴───────────┘
                          │
          ┌───────────────┴───────────────┐
          ▼                               ▼
┌─────────────────┐             ┌─────────────────┐
│      Word       │             │  WordGenerator  │
│  - Display      │◄────────────│  - Random words │
│  - Movement     │             │  - C++20 random │
│  - Color anim   │             └─────────────────┘
└─────────────────┘
          │
          ▼
┌─────────────────┐
│  ScoreManager   │
│  - High scores  │
│  - File I/O     │
└─────────────────┘
```

## Project Structure

```
MonkeyTyper/
├── main.cpp                    # Entry point
├── Game.h/cpp                  # Main controller (State Pattern)
├── IGameState.h                # State interface
├── states/
│   ├── MenuState.h/cpp         # Main menu
│   ├── PlayingState.h/cpp      # Active gameplay
│   ├── GameOverState.h/cpp     # Game over screen
│   └── FontSelectState.h/cpp   # Font selection
├── ResourceManager.h/cpp       # Singleton resource loader
├── AudioManager.h/cpp          # Singleton audio controller
├── Word.h/cpp                  # Word entity
├── WordGenerator.h/cpp         # Random word generator
├── ScoreManager.h/cpp          # High score persistence
├── Config/                     # Configuration files
├── Font/                       # TTF/OTF fonts
├── Sounds/                     # Audio files
└── Textures/                   # Background images
```

## Build Instructions

### Prerequisites

- **C++20 compatible compiler** (GCC 10+, Clang 10+, MSVC 2019+)
- **CMake 3.27+**
- Internet connection for FetchContent (SFML, fmt)

### Building

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release
```

### CLion Users

Simply open the project folder — CLion will automatically configure and build using the existing CMakeLists.txt.

## Controls

| Key | Action |
|-----|--------|
| `Enter` | Start game / Play again |
| `C` | Font selection (from menu) |
| `L` | Load saved game |
| `Ctrl+S` | Save game (during play) |
| `M` | Return to menu (game over) |
| `F1` | Toggle mute |
| `ESC` | Quit |

## Gameplay

1. Words spawn on the left and move right
2. Type the words exactly as shown
3. Words change color (white → yellow → orange → red) as they approach the edge
4. Missing 5 words ends the game
5. Score multiplier increases with progress

## Dependencies

- [SFML 2.6.1](https://www.sfml-dev.org/) — Graphics, Audio, Window
- [fmt 10.2.1](https://fmt.dev/) — String formatting

Both are fetched automatically via CMake FetchContent.

## License

Educational project — all rights reserved. It was developed for 2023/2024 PJC course at PJAIT.

---

*Whole project was hand written by me. The commit hisotry is in another repo, because I had to get rid of the private information and protected data.
