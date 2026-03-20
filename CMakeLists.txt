cmake_minimum_required(VERSION 3.27)
project(MonkeyTyper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY  https://github.com/SFML/SFML
    GIT_TAG         2.6.1
)

# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY  https://github.com/fmtlib/fmt
    GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(SFML fmt)

# Source files - Core
set(CORE_SOURCES
    main.cpp
    Game.cpp
    Game.h
    Word.cpp
    Word.h
    WordGenerator.cpp
    WordGenerator.h
    ScoreManager.cpp
    ScoreManager.h
)

# Source files - State Pattern
set(STATE_SOURCES
    IGameState.h
    states/MenuState.cpp
    states/MenuState.h
    states/PlayingState.cpp
    states/PlayingState.h
    states/GameOverState.cpp
    states/GameOverState.h
    states/FontSelectState.cpp
    states/FontSelectState.h
)

# Source files - Managers
set(MANAGER_SOURCES
    ResourceManager.cpp
    ResourceManager.h
    AudioManager.cpp
    AudioManager.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${CORE_SOURCES}
    ${STATE_SOURCES}
    ${MANAGER_SOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    fmt::fmt 
    sfml-graphics 
    sfml-audio
)

# IDE source groups for better organization
source_group("Core" FILES ${CORE_SOURCES})
source_group("States" FILES ${STATE_SOURCES})
source_group("Managers" FILES ${MANAGER_SOURCES})
