#pragma once

#include <raylib.h>


inline const Color DARKRED = {.r = 131, .g = 12, .b = 0, .a = 255};

inline const Color TRANSDARKRED = {.r = 50, .g = 0, .b = 0, .a = 60};
inline const Color TRANSRED = {.r = 255, .g = 0, .b = 0, .a = 60};

inline const Color CYAN = {.r = 0, .g = 201, .b = 153, .a = 255};
inline const Color DARKYELLOW = {.r = 153, .g = 151, .b = 0, .a = 255};
inline const Color DARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 255};
inline const Color TRANSDARKERGRAY = {.r = 40, .g = 40, .b = 40, .a = 150};

inline const Color TRANSDARKGREEN = {.r = 0, .g = 50, .b = 0, .a = 60};
inline const Color TRANSGREEN = {.r = 0, .g = 255, .b = 0, .a = 60};




//pivot on red  0- 100
inline const int OCEAN_RED_LOW = 0;
inline const Color TERRAINCOLOR_OCEAN = DARKBLUE;

//pivot on red  65 - 125
inline const int GRASS_RED_LOW = 37;
inline const Color TERRAINCOLOR_GRASS = DARKGREEN;

inline const Color TERRAINCOLOR_DIRT = DARKBROWN;

//pivot on all  100 - 200
inline const int ROCK_LOW = 100;
inline const Color TERRAINCOLOR_ROCK = DARKGRAY;