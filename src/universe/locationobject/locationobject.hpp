#pragma once
#include <raylib.h>
#include "../../input/selectionmanager.hpp"
#include <vector>
#include <string>
#include "../systemobject/systemobject.hpp"
#include "../../ships/ships.hpp"



/* struct LocationSiteLocalData {
    int uid = -1;
    std::string name = "no site name";
};
 */
class LocationSite;
struct LocationSiteData {
    LocationSite *site_instance;
    SiteLocalData *local_data;
    int uid = -1;
    int location_uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    std::string name = "no site name";
    Vector2 position;

    float radius = 0.0f;
    //float location_radius = 0.0f;
    float detect_radius = 0.0f;
};

//========================================

class LocationObject  {
    public:
        virtual ~LocationObject() = default;
        virtual void Update() = 0;   
        virtual void Draw() = 0; 
        virtual void DrawOverlay() = 0;
        virtual void DrawUI() = 0;
        virtual float GetRenderScale() = 0;
        virtual void RegisterWithManagers(SelectionManager *sm) = 0;

        bool is_on_screen = false;
        bool y_sort = false;

        MouseTriggerArea info_area;
        Label info_label;

        SelectionManager *selection_manager = nullptr;

        Signal player_approaching;
        Signal player_departing;

        
};



class LocationSite : public LocationObject {
    public:
        LocationSite(LocationSiteData *_data);
        ~LocationSite() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        LocationSiteData *site_data = nullptr;
        SelectionManager *selection_manager = nullptr;
        
        Signal launch_requested;

}; 


class LocationShip : public LocationObject {
    public:
        LocationShip(ShipData *_data, Vector2 position);
        ~LocationShip() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        ShipData *ship_data = nullptr;
        SelectionManager *selection_manager = nullptr;

        Vector2 position;
        bool can_board_ship = false;
        
}; 