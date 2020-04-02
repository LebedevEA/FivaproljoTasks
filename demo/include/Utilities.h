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
// TODO may not work
