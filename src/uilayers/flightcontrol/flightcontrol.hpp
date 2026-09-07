#pragma once
#include "../uilayers.hpp"
#include "autppilot.hpp"


struct UniverseData;  //from universe.hpp

struct NavTargetSharedData {

    SystemSiteData *site = nullptr;
    SystemLocationData *location = nullptr;
    SystemBodyData *body = nullptr;
    Vector2 target_position;
    Vector2 this_position;
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
    bool toggled = false;
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
    void Update(bool focussed);
    void Draw(Vector2 list_position, bool focussed);
    void MakeListAll( );
    void MakeListBodies( );
    void MakeListLocations( );
    void MakeListSites( );

    void Select(NavListEntry &entry);

    NavTargetSharedData *shared_list_data = nullptr;

    std::vector<NavListEntry> master_nav_list;
    std::vector<NavListEntry> display_nav_list;

    //NavListEntry *highlighted_entry;
    //std::vector<Label> label_list;

    Vector2 position;

    bool new_list = false;
    int index = 0;
    int list_size = 0;
    LIST_TYPE list_type = LIST_TYPE::ALL;
    int scroll_index = 0;
    int visible_count = 8;

    Signal select_item;
};

class NavInfoPanel {
    public:
    NavInfoPanel() = default;
    NavInfoPanel(Vector2 position);
    void Draw();
    void UpdateInfo(); 

    //NavTargetSharedData *shared_nav_data;
    NavTargetSharedData *shared_list_data = nullptr;

    Label info_label;
    Label distance_label;
    
};


struct UniverseListEntry {
    Label list_label;
    Label map_label;
    SystemMapData *system = nullptr;
    Vector2 position = {0, 0};
    bool selected = false;

    //std::vector<Vector2> master_connection_points;
    //std::vector<Vector2> display_connection_points;
};

class UniverseList {
    
    public:
    enum LIST_TYPE {
        ALL,
        BODIES,
        LOCATIONS,
        SITES,
    };
    
    UniverseList() = default;
    UniverseList(Rectangle _bounds);
    void Update(bool focussed);
    void Draw(bool focussed);
    void MakeListAll( );

    void Select(UniverseListEntry &entry);

    std::vector<UniverseListEntry> master_system_list;
    std::vector<UniverseListEntry> display_system_list;

    UniverseListEntry *selected_system_data = nullptr;

    Rectangle bounds;

    bool new_list = false;
    int index = 0;
    int list_size = 0;
    LIST_TYPE list_type = LIST_TYPE::ALL;
    int scroll_index = 0;
    int visible_count = 8;

    int current_system_uid = -1;

    Signal select_item;
    
};

class UniverseMap {
    public:
    UniverseMap() = default;
    UniverseMap(Rectangle _bounds);
    void Update();
    void Draw();
    void HandleMapMovement();

    std::vector<UniverseListEntry> *display_system_list = nullptr;
    UniverseListEntry *selected_system_data = nullptr;

    UniverseData *universe = nullptr;

    Rectangle bounds;
    Vector2 center;
    float map_scale = 500.0;

    int current_system_uid = -1;

    float animation_factor = 0.0f;

};




class UniversePanel {
    public:
    UniversePanel() = default;
    UniversePanel(Rectangle _bounds);
    void Draw();
    void Update();
    void CreateUniverseList(UniverseData *_universe, int system_uid);

    UniverseData *universe = nullptr;
    std::unordered_map<int, SystemMapData> *map_data = nullptr;

    UniverseListEntry selected_system_data;

    UniverseList universe_list;
    UniverseMap universe_map;

    Vector2 position;

    Rectangle bounds;
    Vector2 center;
    Rectangle map_frame;
    Rectangle list_frame;

    Button close_button;
    Button set_target_button;

    Signal close_universe_panel;
    Signal set_system_target;

    int current_system_uid;

    bool can_jump = false;

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
    
    enum NAV_COLUMN{
        STATE,
        LIST,
        INFO,
    };

    enum NAV_STATE {
        SYSTEM,
        UNIVERSE
    };
    
    public:
    Navigation(Rectangle min, Rectangle max);
    ~Navigation() override;
    void Update() override;
    void Draw() override;


    void CreateSystemList(System *system);

    void OnTopPanelButtonPressed();

    //void OnNewNavTareget();
    void OnCloseUniversePanel();
    void OnSetSystemTarget();

    void OnSelectItem();

    void SetSharedData(NavTargetSharedData *_shared_nav_data, int sytem_uid);

    NavTargetSharedData *shared_nav_data = nullptr;
    NavTargetSharedData list_data;

    Label nav_target_label;
    Label nav_distance_label;

    ButtonPanel top_panel;
    SharedButtonPayload top_panel_payload;
    SystemList system_list;
    NavInfoPanel info_panel;
    Button nav_button;
    UniversePanel universe_panel;

    std::array <Button, 2> state_buttons;
    int state_button_index = 0;
    //Button system_button;
    //Button universe_button;

    Signal set_nav_target;
    Signal deselect_nav_target;

    Navigation::NAV_COLUMN column = LIST;
    Navigation::NAV_STATE nav_state = SYSTEM;

    //Signal system_object_selected;
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

    NavTargetSharedData *target_data = nullptr;

    Label target_label;
    Label distance_label;
};





class FlightControl : public UILayer {
    public:
        FlightControl();
        ~FlightControl() override;
        void Update() override;
        void Draw() override;

        void SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm, UniverseData *_universe);
        void ClearTarget();

        void OnTargetSelected();
        void OnTargetDeSelected();

        void OnNavTargetDeSelected();
        void OnNavTargetSelected();
        void OnEnterTargetSpace();
        void OnLandingAtTarget();
        void OnAutopilotInitiated();
        void SetAutopilotTarget();



        NavTargetSharedData shared_nav_data;
        NavTargetSharedData shared_target_data;

        CreatureEntity *entity; //this is the entity controlling the ship
        SelectionManager *selection_manager = nullptr; //gets area info and signals out
        System *system = nullptr; // system.map_data has all the data
        UniverseData *universe;
        std::unordered_map<int, SystemMapData> *universe_map; 

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
