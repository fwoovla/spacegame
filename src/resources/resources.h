#pragma once
#include <raylib.h>
#include <raymath.h>
#include "json.hpp"
#include "string"
#include <fstream>



struct Vector3i {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Vector3i& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Vector3iHash {
    std::size_t operator()(const Vector3i& p) const {
        return std::hash<int>{}(p.x) ^
               (std::hash<int>{}(p.y) << 1) ^
               (std::hash<int>{}(p.z) << 2);
    }
};

struct Vector2i {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Vector2i& other) const {
        return x == other.x && y == other.y;
    }
};

struct Vector2iHash {
    std::size_t operator()(const Vector2i& p) const {
        return std::hash<int>{}(p.x) ^
               (std::hash<int>{}(p.y) << 1);
    }
};


struct  RayCast {
    Vector2 position;
    Vector2 direction;
};

struct Polygon {
    std::vector<Vector2> points;
    Color color;
};

struct Line {
    Vector2 start;
    Vector2 end;
};



struct GameSettings {
    std::string title = "no title";
};

extern GameSettings g_game_settings;

extern Font g_font;


void LoadSettings(std::string json_path);

void LoadResources();