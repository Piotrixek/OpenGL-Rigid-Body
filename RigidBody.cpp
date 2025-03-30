// RigidBody.cpp
#include "RigidBody.h"
#include <glm/gtc/quaternion.hpp>
RigidBody::RigidBody(const glm::vec3& pos, float m, float r)
    : position(pos), mass(m), radius(r),
    velocity(0.0f), orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
    angularVelocity(0.0f), restitution(0.5f), friction(0.5f),
    forceAccumulator(0.0f), torqueAccumulator(0.0f) {}

void RigidBody::applyForce(const glm::vec3& force) {
    forceAccumulator += force;
}

void RigidBody::applyTorque(const glm::vec3& torque) {
    torqueAccumulator += torque;
}

void RigidBody::update(float dt) {
    glm::vec3 acceleration = forceAccumulator / mass;
    velocity += acceleration * dt;
    velocity *= 0.99f;
    position += velocity * dt;
    float inertia = 0.4f * mass * radius * radius;
    glm::vec3 angularAcceleration = torqueAccumulator / inertia;
    angularVelocity += angularAcceleration * dt;
    angularVelocity *= 0.99f;
    glm::quat q(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
    orientation = glm::normalize(orientation + 0.5f * q * orientation * dt);
    forceAccumulator = glm::vec3(0.0f);
    torqueAccumulator = glm::vec3(0.0f);
}
