#pragma once
#include "../utils/utils.hpp"
//#include <raymath.h>

#include "../utils/utils.hpp"
#include "../areas/areas.hpp"



struct AreaResult
{
    MouseTriggerArea* area = nullptr;
    float distance = std::numeric_limits<float>::max();
    int priority = 0;
};

/* struct BodySelectionResult {
    MouseTriggerArea* area = nullptr;
    int body_uid = -1;
    int location_uid = -1;
    int site_uid = -1;
};
 */

class SelectionManager {
    public:
    SelectionManager();
    ~SelectionManager() {};
    void Update();
    void DrawUI();
    AreaResult GetSelection(Vector2 mouse_pos);

    void Register(MouseTriggerArea *area);
    void Unregister(MouseTriggerArea *area);

    //void OnAreaSelected();

    std::vector<MouseTriggerArea*> areas;

    MouseTriggerArea* current_hover = nullptr;

    MouseTriggerArea* selection = nullptr;

    Signal selected;
    Signal deselected;

};