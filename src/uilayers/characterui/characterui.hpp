#pragma once
#include "../uilayers.hpp"



struct UniverseData;  //from universe.hpp

/* 
enum C_COMPONENT_STATE {
    HIDDEN,
    MINIMIZED,
    FOCUSED
};
 */
class CharacterUIComponent : public UILayer {
    public:
    ~CharacterUIComponent() = default;
    virtual void SetState(COMPONENT_STATE new_state) = 0;
    COMPONENT_STATE state = MINIMIZED;
    Rectangle min_bounds;
    Rectangle max_bounds;
    Label top_label;
};




class CharacterNavigation : public CharacterUIComponent{
    
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
    CharacterNavigation(Rectangle min, Rectangle max);
    ~CharacterNavigation() override;
    void Update() override;
    void Draw() override;
    void SetState(COMPONENT_STATE new_state) override;

    //void CreateSiteList(Location *location);

};






class CharacterInfo : public CharacterUIComponent{
    public:
    CharacterInfo(Rectangle min, Rectangle max);
    ~CharacterInfo() override;
    void Update() override;
    void Draw() override;
    void SetState(COMPONENT_STATE new_state) override;

    Character *character = nullptr;

    Label entity_label;
    
};




class CharacterUI : public UILayer {
    public:
    CharacterUI();
    ~CharacterUI() {};
    void Update() override;
    void Draw() override;

    void SetTarget(CreatureEntity *_entity, System *sys, SelectionManager *sm, UniverseData *_universe);
    void ClearTarget();

    void OnTargetSelected();
    void OnTargetDeSelected();


    CreatureEntity *entity; //this is the entity controlling the ship
    SelectionManager *selection_manager = nullptr; //gets area info and signals out
    System *system = nullptr; // system.map_data has all the data
    UniverseData *universe;
    std::unordered_map<int, SystemMapData> *universe_map;
    //Location



    CharacterNavigation *navigation;
    CharacterInfo *character_info;

    std::vector<std::unique_ptr<CharacterUIComponent>> components;

    CharacterUIComponent *focused_component = nullptr;
};