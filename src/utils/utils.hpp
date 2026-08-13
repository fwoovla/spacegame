#pragma once

#include <vector>
#include <raylib.h>
#include <cmath>
#include <functional>



#define MAX_SIGNAL_CONNECTIONS 256

int GetUID();


enum STAT_ID {
    STAT_DAMAGE,
    STAT_PPS,
    STAT_CHARGE,
    STAT_KNOCKBACK,
    STAT_RECOIL,
    STAT_ACCURACY,
    STAT_MAX_POWER,
    STAT_COUNT
};

struct Limit {
    float min = 0.0f;
    float max = 0.0f;
};

enum SIGNAL {
    END_GAME,
    TIMER_TIMEOUT,
    PLAY_PRESSED,
    SHOULD_DELETE,
    CAN_FIRE
};

class Signal {
    public:
    int Connect(std::function<void()> const& callback);
    void Disconnect(int id);
    void DisconnectAll();
    void EmitSignal();    


    struct Connection
    {
        int id;
        std::function<void()> callback;
    };

    std::vector<Connection> connections;

    int next_id = 0;
};


class Timer{
    public:
    void Start(double _wait_time, bool _one_shot);
    void Stop();
    void Update(); //false if finished
    bool IsDone();
    bool IsActive();
    double TimeRemaining();
    double TimeElapsed();
    double GetWaitTime();
    
    Signal timer_timeout;

    double wait_time; //in seconds
    private:
    double elapsed_time;
    bool active;
    bool finished;
    bool one_shot;    
};





Color BlendColors(Color a, Color b, float t);

Vector2 vClamp(Vector2 v, float maxLength);

float GetAngleFromTo(Vector2 from, Vector2 to);


float WrapAngle(float a);


float ShortestAngleDiffRad(float from, float to);


float RotateTowardsRad(float current, float target, float maxTurn, float dt);

float RectCenterDistSq(const Rectangle& a, const Rectangle& b);