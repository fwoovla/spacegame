#pragma once
#include <raylib.h>
#include <raymath.h>


struct SpriteDrawData
{
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 center;
    float rotation;
    Color modulate;
};


struct Sprite {
    Texture2D texture;
    Vector2 position = Vector2{0,0};
    Vector2 center = Vector2{0,0};;
    Vector2 size = Vector2{0,0};;
    Rectangle source = Rectangle{0,0,0,0};
    Rectangle dest = Rectangle{0,0,0,0};
    Vector2 scale = Vector2{1,1};
    float rotation = 0.0f;
    Color modulate = WHITE;
    float frame_size = 0.0f;
    Vector2 pivot = Vector2{0,0};
};

struct AnimatedSprite : public Sprite {

    int max_frames = 1;
    int frame = 0;
    int y_index = 0;
    float frame_delay = 0.0f;
    double last_frame_time = 0.0;
    
};

struct EnvironmentalSprite : public Sprite {

    bool fadeable = false;
    
};

void LoadSpriteCentered(Sprite &_sprite, Texture2D _texture, Vector2 _position);
void LoadSpriteAnchored(Sprite &_sprite, Texture2D _texture, Vector2 _position);


void LoadSpriteCentered(AnimatedSprite &_sprite, Texture2D _texture, Vector2 _position, int _max_frames, float _frame_size, float _frame_delay);
void LoadSpriteAnchored(AnimatedSprite &_sprite, Texture2D _texture, Vector2 _position, int _max_frames, float _frame_size, float _frame_delay);


void LoadSprite(Sprite &_sprite, Texture2D _texture, Vector2 _position);
void LoadSprite(AnimatedSprite &_sprite, Texture2D _texture, Vector2 _position, int _max_frames, float _frame_size, float _frame_delay);



void ScaleSprite(Sprite &_sprite, Vector2 _scale);


void AnimateSprite(AnimatedSprite &_sprite);

void DrawSprite(Sprite &_sprite);

void DrawSprite(AnimatedSprite &_sprite);

void DrawSprite(EnvironmentalSprite &_sprite);

void SetAmination(AnimatedSprite &_sprite, int _y_index);