#pragma once
#include "../resources/resources.h"
#include "../universe/entity.hpp"
#include "../controllers/controllers.hpp"
#include "../controllers/controllers.hpp"

void LoadData();

void LoadCreatureEntityData(std::string file_path);

void LoadObjectEntityData(std::string file_path);


void LoadShipControllerData(std::string file_path);

void LoadCharacterControllerData(std::string file_path);

void LoadObjectControllerData(std::string file_path);