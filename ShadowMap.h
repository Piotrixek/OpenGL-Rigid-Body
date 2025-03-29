// ShadowMap.h
#pragma once
#include <glad/glad.h>
class ShadowMap {
public:
    unsigned int depthMapFBO;
    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    ShadowMap();
    void init();
};
