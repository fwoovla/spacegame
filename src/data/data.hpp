#pragma once
#include "../resources/resources.h"
#include "../universe/entity.hpp"
#include "../controllers/controllers.hpp"
//#include "../controllers/controllers.hpp"

void LoadData();

void LoadCreatureEntityData(std::string file_path);

void LoadObjectEntityData(std::string file_path);


void LoadShipData(std::string file_path);
void LoadShipEquipment(std::string file_path);

SystemDriveData LoadSystemDriveData(json &e);

void LoadCharacterControllerData(std::string file_path);

void LoadObjectControllerData(std::string file_path);