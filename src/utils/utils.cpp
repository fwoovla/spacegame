#include "utils.hpp"
#include <algorithm>


int GetUID() {
    return GetRandomValue(100, 10000000);
}



Vector2 vClamp(Vector2 v, float maxLength) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length > maxLength) {
        float scale = maxLength / length;
        v.x *= scale;
        v.y *= scale;
    }
    return v;
}

float GetAngleFromTo(Vector2 from, Vector2 to) {
    Vector2 dir = { to.x - from.x, to.y - from.y };
    float angle = atan2(dir.y, dir.x);
    return angle;
}

float WrapAngle(float a) {
    a = std::fmod(a + PI, 2*PI);
    if (a < 0) a += 2*PI;
    return a - PI;
}

float ShortestAngleDiffRad(float from, float to) {
    return WrapAngle(to - from);
}


 float RotateTowardsRad(float current, float target, float maxTurn, float dt) {
    float diff = ShortestAngleDiffRad(current, target);
    float maxStep = maxTurn * dt;
    if (std::fabs(diff) <= 1e-4f) return target;
    if (std::fabs(diff) <= maxStep) return target;
    return current + (diff > 0 ? 1 : -1) * maxStep;
}


float RectCenterDistSq(const Rectangle& a, const Rectangle& b)
{
    float ax = a.x + a.width  * 0.5f;
    float ay = a.y + a.height * 0.5f;
    float bx = b.x + b.width  * 0.5f;
    float by = b.y + b.height * 0.5f;

    float dx = ax - bx;
    float dy = ay - by;

    return dx*dx + dy*dy;
}


/* void DrawLineLine(Line &line, Color &color) {
    DrawLineV(line.start, line.end, color);
}

Color BlendColors(Color a, Color b, float t)
{
    t = std::clamp(t, 0.0f, 1.0f);

    return {
        (unsigned char)(a.r * (1.0f - t) + b.r * t),
        (unsigned char)(a.g * (1.0f - t) + b.g * t),
        (unsigned char)(a.b * (1.0f - t) + b.b * t),
        (unsigned char)(a.a * (1.0f - t) + b.a * t)
    };
} */