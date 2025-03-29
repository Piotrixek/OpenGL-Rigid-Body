// FPSCounter.cpp
#include "FPSCounter.h"
FPSCounter::FPSCounter() : fps(0.0f) {}
void FPSCounter::update(float deltaTime) {
    fps = 1.0f / deltaTime;
}
