#pragma once

namespace Utilities {
    
enum class Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    UNKNOWN
};

enum class Color {
    NONE,
    GREEN,
    YELLOW
};

enum class GameState {
    SINGLE_PLAYER,
    MULTI_PLAYER,
    MENU,
    EXIT
};
    
} // end namespace Utilities

namespace KeyPresserUtility {

enum class ManipulatorType {
    PLAYER,
    MENU
};
    
} // end namespace KeyPresserUtilitie

#define unused(p) (void)p
