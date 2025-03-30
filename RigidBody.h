// RigidBody.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::quat orientation;
    glm::vec3 angularVelocity;
    float mass;
    float radius;
    float restitution;
    float friction;
    RigidBody(const glm::vec3& pos, float mass, float radius);
    void applyForce(const glm::vec3& force);
    void applyTorque(const glm::vec3& torque);
    void update(float dt);
private:
    glm::vec3 forceAccumulator;
    glm::vec3 torqueAccumulator;
};
