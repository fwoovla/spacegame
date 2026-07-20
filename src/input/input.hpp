#pragma once

#include <raylib.h>

#define KEY_BUFFER_SIZE 10

struct PlayerInput {
    Vector2 screen_mouse_position;
    Vector2 world_mouse_position;
    Vector2 mouse_left_clicked_screen_position;
    Vector2 mouse_left_clicked_world_position;
    Vector2 mouse_right_clicked_screen_position;
    Vector2 mouse_right_clicked_world_position;

    bool mouse_left;
    bool mouse_left_down;
    bool mouse_right;
    bool mouse_right_down;
    bool mouse_middle;
    bool mouse_middle_down;
    float mouse_wheel;
    bool selecting;
    Rectangle selected_rect;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    bool key_sprint;

    bool key_use;

    bool key_switch_weapon;
    bool key_reload;

    KeyboardKey keys_pressed[10];
};

extern PlayerInput g_input;

void GetInputFromPlayer();