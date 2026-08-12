#pragma once
#include "../uilayers.hpp"


enum COMPONENT_STATE {
    HIDDEN,
    MINIMIZED,
    FOCUSED
};

class FlightComponent : public UILayer {
    public:
    virtual ~FlightComponent() = default;
    COMPONENT_STATE state = MINIMIZED;
    Rectangle min_bounds;
    Rectangle max_bounds;
    Label top_label;
};


struct ListSite {
    SystemSiteData *site = nullptr;
};


struct ListLocation {
    SystemLocationData *location = nullptr;
    std::vector<ListSite> site_list;
};

struct ListBody {

    SystemBodyData *body = nullptr;
    std::vector<ListLocation> location_list;
};

struct SystemList {

    std::vector<ListBody> body_list;

};


struct NavTargetSharedData {

    SystemSiteData *site = nullptr;
    SystemLocationData *location = nullptr;
    SystemBodyData *body = nullptr;

};



class Navigation : public FlightComponent{
    public:
    Navigation(Rectangle min, Rectangle max);
    ~Navigation() override;
    void Update() override;
    void Draw() override;


    void CreateSystemList(System *system);
    //void SelectSystemObject();

    NavTargetSharedData *nav_target_data = nullptr;

    Label nav_target_label;
    Label nav_distance_label;

    SystemList system_list;

    Signal system_object_selected;
    
};



class ShipInfo : public FlightComponent{
    public:
    ShipInfo(Rectangle min, Rectangle max);
    ~ShipInfo() override;
    void Update() override;
    void Draw() override;

    Label entity_label;
    
};



class TargetScreen : public FlightComponent {
    public:
    TargetScreen(Rectangle min, Rectangle max);
    ~TargetScreen() override;
    void Update() override;
    void Draw() override;

    NavTargetSharedData *nav_target_data = nullptr;

    Label target_label;
    Label distance_label;
};





class FlightControl : public UILayer {
    public:
        FlightControl();
        ~FlightControl() override;
        void Update() override;
        void Draw() override;

        void SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm);
        void ClearTarget();

        void OnSystemObjectSelected();
        void OnSystemObjectDeSelected();

        NavTargetSharedData shared_nav_target_data;

        CreatureEntity *entity; //this is the entity controlling the ship
        SelectionManager *selection_manager = nullptr; //gets area info and signals out
        System *system = nullptr; // system.map_data has all the data


        Navigation *navigation;
        TargetScreen *target_screen;
        ShipInfo *ship_info;

        std::vector<std::unique_ptr<FlightComponent>> components;

        FlightComponent *focused_component = nullptr;


        Label throttle_label;

        Rectangle autopiolot_indicator;
        Label autopilot_label;

        Rectangle flight_assist_indicator;
        Label flight_assist_label;


};
