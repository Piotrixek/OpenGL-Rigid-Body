// RigidBody.cpp
#include "RigidBody.h"
#include <glm/gtc/quaternion.hpp>
RigidBody::RigidBody(const glm::vec3& pos, float m, float r)
    : position(pos), mass(m), radius(r), velocity(0.0f), acceleration(0.0f),
    orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), angularVelocity(0.0f),
    restitution(0.5f), friction(0.5f) {}
void RigidBody::applyForce(const glm::vec3& force) {
    acceleration += force / mass;
}
void RigidBody::applyTorque(const glm::vec3& torque) {
    angularVelocity += torque / mass;
}
void RigidBody::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = glm::vec3(0.0f);
    glm::quat q(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z);
    orientation = glm::normalize(orientation + 0.5f * q * orientation * dt);
}
