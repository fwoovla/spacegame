#pragma once

#include <memory>
#include <raylib.h>
#include "../universe/system.hpp"
#include "../ui/label.hpp"
#include "../ui/button.hpp"
#include "../ui/textinput.hpp"
#include "../universe/entity.hpp"
#include "../ships/ships.hpp"
//#include "../universe/components/components.hpp"
#include "../colordefs.hpp"


class UILayer{
    public:
    virtual ~UILayer(){};
    virtual void Update() = 0;
    virtual void Draw() = 0;
};


class DebugUiLayer : public UILayer {
    public:

    DebugUiLayer();
    ~DebugUiLayer() override;
    void Update() override;
    void Draw() override;

    Label header_label;
    Label render_time_label;
    Label update_time_label;
    Label entity_count_label;
    Label tile_count_label;
    Label chunk_count_label;
    Label fps_label;
    Label player_layer_label;
    Label player_chunk_label;
    Label player_grid_label;
    Label player_world_label;
    Label imp;
    Label wmp;
    Label selected_world_pos;


};



class TitleUiLayer : public UILayer {
    public:

    TitleUiLayer();
    ~TitleUiLayer() override;
    void Update() override;
    void Draw() override;

    Label scene_label;
    Label version_label;  

    Button start_button;
    Button exit_button;
    Button settings_button;

    Label name_label;
    TextInput input;

    

};

class GameUiLayer : public UILayer {
    public:

    GameUiLayer();
    ~GameUiLayer() override;
    void Update() override;
    void Draw() override;

    Label scene_label;
};



class LocationTestSceneUiLayer : public UILayer {
    public:

    LocationTestSceneUiLayer();
    ~LocationTestSceneUiLayer() override;
    void Update() override;
    void Draw() override;

    Label size_x_label;
    TextInput size_x_input;

    Label size_y_label;
    TextInput size_y_input;

    Label grid_size_label;
    TextInput grid_size_input;
};