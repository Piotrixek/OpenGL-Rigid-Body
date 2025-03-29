// ParticleSystem.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    float life;
};
class ParticleSystem {
public:
    std::vector<Particle> particles;
    ParticleSystem(unsigned int count);
    void update(float dt);
    void draw();
private:
    unsigned int particleVAO, particleVBO;
    void init();
};
