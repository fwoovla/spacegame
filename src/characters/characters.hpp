#pragma once 
#include <raylib.h>
//#include "equipmenttemplates.hpp"
#include "../resources/resources.h"
#include "../controllers/controllers.hpp"


enum CHARACTER_ID {
    CHARACTER_NONE = -1,
    CHARACTER_PLAYER,
};



struct CharacterTemplateData {
    std::string name = "no name";
    CHARACTER_ID id = CHARACTER_NONE;
    int credits = 0;
    
    //CharacterMovement movement;
    float speed = 0.0f;

    float radius = 0.0f;
};

extern std::unordered_map<int, CharacterTemplateData> g_character_template_data;


struct CharacterData {
    int uid;
    std::string name = "no name";
    CHARACTER_ID id = CHARACTER_NONE;

    //CharacterMovement movement;
    float speed = 0.0f;

    float radius = 0.0f;
};

CHARACTER_ID StrToCharacterId(std::string str);


class CharacterController;
class Character {
    public:
    Character(CharacterData *_data);
    ~Character() = default;

    void Update(Vector2 &position);
    void Draw(Vector2 &position, float scale);

    CharacterData *character_data = nullptr;
    std::unique_ptr<CharacterController> character_controller;
};