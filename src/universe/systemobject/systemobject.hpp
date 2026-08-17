#pragma once
#include <raylib.h>
#include "../../input/selectionmanager.hpp"
#include <vector>
#include <string>


struct SystemSiteLocalData {
    int uid = -1;
    std::string name = "no site name";
};


struct SystemLocationLocalData {
    int uid = -1;
    std::string name = "no location name";
};


struct SystemBodyLocalData {
    int uid = -1;
    std::string name = "no body name";
};


enum BODY_TYPE {
    BODY_STAR,
    BODY_PLANET,
    BODY_MOON,
};


class SystemBody;
struct SystemBodyData {
    SystemBody *body_instance;
    SystemBodyLocalData local_data;  //economy population etc...

    int uid = -1;
    int system_uid;
    std::string name = "no body name";

    //SystemBodyMapData map_data;

    Vector2 position;

    float radius = 0.0f;
    float detect_radius = 0.0f;
    float orbit_radius = 0.0f;
    float orbit_angle = 0.0f;

    BODY_TYPE body_type;

    bool landable = false;
    
    int orbital_layer_count = 0;
    float orbital_layer_delta;
    int orbital_body_count = 0;
    std::vector<int> orbital_body_uids;
    std::vector<int> location_uids;
    
    bool obstructable = false;
    Color modulate;
    
    int parent_orbital = 0;  //where it is in relation to it's parent orbitals
    int parent_uid = -1;

    
};

class SystemLocation;
struct SystemLocationData {
    SystemLocation *location_instance;
    SystemLocationLocalData local_data;  //economy population etc...

    int uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    std::string name = "no location name";
    Vector2 position;

    float system_radius = 0.0f;
    float location_radius = 0.0f;
    float detect_radius = 0.0f;

};

class SystemSite;
struct SystemSiteData {
    SystemSite *site_instance;
    SystemSiteLocalData local_data;
    int uid = -1;
    int location_uid = -1;
    int body_uid = -1;
    int system_uid = -1;
    std::string name = "no site name";
    Vector2 position;

    float system_radius = 0.0f;
    float location_radius = 0.0f;
    float detect_radius = 0.0f;
};


//==========================================================================




class SystemObject  {
    public:
        virtual ~SystemObject() = default;
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



class SystemBody : public SystemObject {
    public:
        SystemBody(SystemBodyData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemBody() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemBodyData *body_data = nullptr;

        SystemBodyData *parent_data = nullptr;
}; 


class SystemLocation : public SystemObject {
    public:
        SystemLocation(SystemLocationData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemLocation() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemLocationData *location_data = nullptr;

        SystemBody *parent;
}; 

class SystemSite : public SystemObject {
    public:
        SystemSite(SystemSiteData *_data);
        //void GenerateLocation(LocationMapData &location);
        ~SystemSite() = default;
        void Update() override;
        void Draw() override;        
        void DrawOverlay() override;
        void DrawUI()override;

        float GetRenderScale() override;
        void RegisterWithManagers(SelectionManager *sm) override;

        SystemSiteData *site_data = nullptr;

        SystemBody *parent;
}; 