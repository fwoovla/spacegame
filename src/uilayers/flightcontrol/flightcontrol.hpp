#pragma once
#include "../uilayers.hpp"
#include "autppilot.hpp"


struct NavTargetSharedData {

    SystemSiteData *site = nullptr;
    SystemLocationData *location = nullptr;
    SystemBodyData *body = nullptr;
    Vector2 target_position;
    bool set = false;

};


struct SharedButtonPayload {
    int payload_i;
    std::string payload_s = "";
};

struct PanelButton {
    Button button;
    int button_index = 0;
    KeyboardKey key;
    std::string payload_s = "";
};

class ButtonPanel : public UILayer {
    public:
    ButtonPanel();
    void Update() override;
    void Draw() override;

    void AddButton(Button button, KeyboardKey key, std::string payload_s);
    void ClearButtons();

    std::vector<PanelButton> buttons;

    SharedButtonPayload *button_payload = nullptr;
    

    Signal button_pressed;
};

struct NavListEntry {
    Label label;
    SystemSiteData *site = nullptr;
    SystemLocationData *location = nullptr;
    SystemBodyData *body = nullptr;
};


class SystemList {
    
    public:
    enum LIST_TYPE {
        ALL,
        BODIES,
        LOCATIONS,
        SITES,
    };
    
    SystemList() = default;
    SystemList(Vector2 list_positon);
    void Update( );
    void Draw(Vector2 list_position);
    void MakeListAll( );
    void MakeListBodies( );
    void MakeListLocations( );
    void MakeListSites( );

    void Select(NavListEntry &entry);

    NavTargetSharedData *shared_nav_data = nullptr;

    std::vector<NavListEntry> master_nav_list;
    std::vector<NavListEntry> display_nav_list;

    NavListEntry *highlighted_entry;
    //std::vector<Label> label_list;

    Vector2 position;

    bool new_list = false;
    int index = 0;
    int list_size = 0;
    LIST_TYPE list_type = LIST_TYPE::ALL;
    int scroll_index = 0;
    int visible_count = 8;

    Signal set_new_target;
    Signal deselect_nav_target;
};




//========================= components =========================





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



class Navigation : public FlightComponent{
    public:
    Navigation(Rectangle min, Rectangle max);
    ~Navigation() override;
    void Update() override;
    void Draw() override;


    void CreateSystemList(System *system);

    void OnTopPanelButtonPressed();
    //void OnNewNavTareget();
    //void SelectSystemObject();

    NavTargetSharedData *shared_nav_data = nullptr;

    Label nav_target_label;
    Label nav_distance_label;

    ButtonPanel top_panel;
    SharedButtonPayload top_panel_payload;
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
        void OnNavTargetDeSelected();
        void OnEnterTargetSpace();
        void OnLandingAtTarget();
        void OnAutopilotInitiated();



        NavTargetSharedData shared_nav_data;

        CreatureEntity *entity; //this is the entity controlling the ship
        SelectionManager *selection_manager = nullptr; //gets area info and signals out
        System *system = nullptr; // system.map_data has all the data

        Navigation *navigation;
        TargetScreen *target_screen;
        ShipInfo *ship_info;

        std::vector<std::unique_ptr<FlightComponent>> components;

        FlightComponent *focused_component = nullptr;

        Label throttle_label;
        Label speed_label;

        Rectangle autopiolot_indicator;
        Label autopilot_label;

        Rectangle flight_assist_indicator;
        Label flight_assist_label;

        Rectangle flight_mode_indicator;
        Label flight_mode_label;
};
