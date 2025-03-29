// FPSCounter.h
#pragma once
class FPSCounter {
public:
    float fps;
    FPSCounter();
    void update(float deltaTime);
};
