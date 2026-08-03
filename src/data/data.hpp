#pragma once
#include "../resources/resources.h"
#include "../ships/ships.hpp"
#include "../characters/characters.hpp"


void LoadData();

void LoadEntityData(std::string file_path);

void LoadShipData(std::string file_path);

void LoadCharacterData(std::string file_path);