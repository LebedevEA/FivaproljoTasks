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
        // ...
    };
    
} // end namespace Utilities

#define unused(p) (void)p
