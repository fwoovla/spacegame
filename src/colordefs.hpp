#pragma once

#include <raylib.h>
#include <vector>


inline const Color DARKRED = {.r = 131, .g = 12, .b = 0, .a = 255};

inline const Color TRANSDARKRED = {.r = 50, .g = 0, .b = 0, .a = 60};
inline const Color TRANSRED = {.r = 255, .g = 0, .b = 0, .a = 60};

inline const Color CYAN = {.r = 0, .g = 201, .b = 153, .a = 255};
inline const Color DARKYELLOW = {.r = 153, .g = 151, .b = 0, .a = 255};
inline const Color DARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 255};
inline const Color TRANSDARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 150};

inline const Color TRANSDARKGREEN = {.r = 0, .g = 50, .b = 0, .a = 60};
inline const Color TRANSGREEN = {.r = 0, .g = 255, .b = 0, .a = 60};


inline const Color SPACEBLUE = {.r = 5, .g = 9, .b = 20, .a = 255};






inline const Color PLANET_BLUE        = {.r = 70,  .g = 130, .b = 200, .a = 255};
inline const Color PLANET_DARKBLUE    = {.r = 35,  .g = 75,  .b = 145, .a = 255};
inline const Color PLANET_CYAN        = {.r = 60,  .g = 180, .b = 180, .a = 255};
inline const Color PLANET_TEAL        = {.r = 55,  .g = 145, .b = 135, .a = 255};

inline const Color PLANET_GREEN       = {.r = 70,  .g = 150, .b = 80,  .a = 255};
inline const Color PLANET_DARKGREEN   = {.r = 40,  .g = 100, .b = 50,  .a = 255};
inline const Color PLANET_OLIVE       = {.r = 125, .g = 135, .b = 65, .a = 255};

inline const Color PLANET_RED         = {.r = 180, .g = 65,  .b = 45,  .a = 255};
inline const Color PLANET_DARKRED     = {.r = 115, .g = 40,  .b = 35, .a = 255};
inline const Color PLANET_ORANGE      = {.r = 200, .g = 115, .b = 45, .a = 255};
inline const Color PLANET_YELLOW      = {.r = 200, .g = 175, .b = 65, .a = 255};

inline const Color PLANET_SAND        = {.r = 190, .g = 165, .b = 110, .a = 255};
inline const Color PLANET_BROWN       = {.r = 125, .g = 85,  .b = 55,  .a = 255};
inline const Color PLANET_DARKBROWN   = {.r = 75,  .g = 50,  .b = 35,  .a = 255};

inline const Color PLANET_PURPLE      = {.r = 125, .g = 75,  .b = 155, .a = 255};
inline const Color PLANET_DARKPURPLE  = {.r = 70,  .g = 40,  .b = 90,  .a = 255};
inline const Color PLANET_PINK        = {.r = 190, .g = 100, .b = 140, .a = 255};

inline const Color PLANET_GRAY        = {.r = 125, .g = 125, .b = 130, .a = 255};
inline const Color PLANET_DARKGRAY    = {.r = 70,  .g = 70,  .b = 75,  .a = 255};
inline const Color PLANET_ICE         = {.r = 150, .g = 205, .b = 220, .a = 255};

inline const std::vector<Color> g_planet_colors = {
    PLANET_BLUE,
    PLANET_DARKBLUE,
    PLANET_CYAN,
    PLANET_TEAL,
    PLANET_GREEN,
    PLANET_DARKGREEN,
    PLANET_OLIVE,
    PLANET_RED,
    PLANET_DARKRED,
    PLANET_ORANGE,
    PLANET_YELLOW,
    PLANET_SAND,
    PLANET_BROWN,
    PLANET_DARKBROWN,
    PLANET_PURPLE,
    PLANET_DARKPURPLE,
    PLANET_PINK,
    PLANET_GRAY,
    PLANET_DARKGRAY,
    PLANET_ICE
};
