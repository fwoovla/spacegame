#include "../entity.hpp"
#include "../../game.h"


AsteroidEntity::AsteroidEntity(EntityData *_data) : ObjectEntity(_data) {


    lifetime_timer.timer_timeout.Connect([this]() {
        printf("AsteroidEntity lifetime timer expired, marking for deletion\n");
        this->should_delete = true;
    });

    lifetime_timer.Start(10.0 + GetRandomValue(0, 10) * 0.1, true);


    //entity_data = _data;

    ObjectEntityControllerTemplateData Tdata = g_object_entity_controller_template_data[entity_data->object_entity_controller_id];
    object_controller_data.id = Tdata.id;
    object_controller_data.name = Tdata.name;
    object_controller_data.movement = Tdata.movement;
    object_controller_data.radius = Tdata.radius;
    object_controller = std::make_unique<ObjectEntityController>(&object_controller_data); 
}

/* AsteroidEntity::~AsteroidEntity() {
    if (selection_manager){
        //selection_manager->Unregister(&info_area);
    }

} */

void AsteroidEntity::Update() {
    lifetime_timer.Update();
    object_controller->Update(entity_data->position);
    //printf("AsteroidEntity update, lifetime timer: %0.3f seconds remaining\n", lifetime_timer.TimeRemaining());
}

void AsteroidEntity::Draw() {
    object_controller->Draw(entity_data->position, 1.0f);
}

void AsteroidEntity::DrawOverlay() {
    object_controller->Draw(entity_data->position, GetRenderScale());

}

void AsteroidEntity::DrawUI() {

}

float AsteroidEntity::GetRenderScale() {
    return 1.0f;
}
void AsteroidEntity::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}