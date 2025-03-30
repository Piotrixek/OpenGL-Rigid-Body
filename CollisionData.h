// CollisionData.h
#pragma once
#include <glm/glm.hpp>
#include "RigidBody.h"
struct CollisionData {
    RigidBody* bodyA;
    RigidBody* bodyB;
    glm::vec3 contactPoint;
    glm::vec3 contactNormal;
    float penetrationDepth;
};
