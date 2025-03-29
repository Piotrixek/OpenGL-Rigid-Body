// PhysicsEngine.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "RigidBody.h"
class PhysicsEngine {
public:
    std::vector<RigidBody*> bodies;
    glm::vec3 windForce = glm::vec3(0.0f);
    float gravityMultiplier = 1.0f;
    void addBody(RigidBody* body);
    void update(float dt);
private:
    void checkCollisions();
};
