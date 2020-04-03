#pragma once

enum class Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    UNKNOWN
};

enum class BlockColor {
    NONE,
    GREEN,
    // ...
};

#define unused(p) (void)p

enum class MenuRetVal {
    RUN_SINGLEPLAYER,
    RUN_MILTUPLAYER,
    EXIT,
    NONE
};
