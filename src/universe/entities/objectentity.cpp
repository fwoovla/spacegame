/* #include "../entity.hpp"
#include "../../game.h"



ObjectEntity::ObjectEntity(EntityData *_data) : BaseEntity(_data) {
    //entity_data = _data;

    ObjectEntityControllerTemplateData Tdata = g_object_entity_controller_template_data[entity_data->object_entity_controller_id];
    object_controller_data.id = Tdata.id;
    object_controller_data.name = Tdata.name;
    object_controller_data.movement = Tdata.movement;
    object_controller_data.radius = Tdata.radius;

    object_controller = std::make_unique<ObjectEntityController>(&object_controller_data);
}

/* ObjectEntity::~ObjectEntity() {
    if (selection_manager){
        //selection_manager->Unregister(&info_area);
    }

}

void ObjectEntity::Update() {
    //printf("\n\n\nObjectEntity update %i\n", entity_data->render_mode);
    object_controller->Update(entity_data->position);
}

void ObjectEntity::Draw() {
    //printf("object Draw -obj\n");
    object_controller->Draw(entity_data->position, 1.0f);
}

void ObjectEntity::DrawOverlay() {
    //printf("object Draw overlay -obj\n");
    object_controller->Draw(entity_data->position, 1.0f);

}

void ObjectEntity::DrawUI() {

}


float ObjectEntity::GetRenderScale() {

    return 1.0f;
}


void ObjectEntity::RegisterWithManagers(SelectionManager *sm) {
    selection_manager = sm;
    selection_manager->Register(&info_area);

}
 */