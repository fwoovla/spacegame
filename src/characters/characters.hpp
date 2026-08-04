#pragma once 
#include <raylib.h>
#include "../resources/resources.h"
#include "../universe/components/components.hpp"


enum CHARACTER_ID {
    CHARACTER_NONE = -1,
    CHARACTER_PLAYER,
};



struct CharacterMovement {
    float speed = 0.0f;
    Vector2 velocity = {0,0};
};



struct CharacterTemplateData {
    std::string name = "no name";
    CHARACTER_ID id;
    int credits = 0;
    
    CharacterMovement movement;

    bool obstructable;
};

extern std::unordered_map<int, CharacterTemplateData> g_character_template_data;


struct CharacterData {
    int uid;
    std::string name = "no name";
    CHARACTER_ID id;

    CharacterMovement movement;
};


class Character {
    
    public:
        Character(CharacterData _data);
        ~Character();
        void Update(Vector2 &position);
        void Draw(Vector2 &position, float scale);

        CharacterData character_data;
};


CHARACTER_ID StrToCharacterId(std::string str);