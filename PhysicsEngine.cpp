// PhysicsEngine.cpp
#include "PhysicsEngine.h"
#include <glm/glm.hpp>
#include <algorithm>
void PhysicsEngine::addBody(RigidBody* body) {
    bodies.push_back(body);
}
void PhysicsEngine::checkCollisions() {
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            RigidBody* a = bodies[i];
            RigidBody* b = bodies[j];
            glm::vec3 diff = b->position - a->position;
            float dist = glm::length(diff);
            float penetration = a->radius + b->radius - dist;
            if (penetration > 0.0f) {
                glm::vec3 collisionNormal = glm::normalize(diff);
                float relativeVelocity = glm::dot(b->velocity - a->velocity, collisionNormal);
                if (relativeVelocity < 0.0f) {
                    float e = std::min(a->restitution, b->restitution);
                    float jImpulse = -(1.0f + e) * relativeVelocity / (1.0f / a->mass + 1.0f / b->mass);
                    glm::vec3 impulse = jImpulse * collisionNormal;
                    a->velocity -= impulse / a->mass;
                    b->velocity += impulse / b->mass;
                }
                float correction = penetration * 0.5f;
                a->position -= collisionNormal * correction;
                b->position += collisionNormal * correction;
            }
        }
    }
}
void PhysicsEngine::update(float dt) {
    for (auto& body : bodies) {
        body->applyForce(glm::vec3(0.0f, -9.81f * gravityMultiplier * body->mass, 0.0f));
        body->applyForce(windForce);
        body->update(dt);
        if (body->position.y - body->radius < 0.0f) {
            body->position.y = body->radius;
            if (body->velocity.y < 0.0f) body->velocity.y *= -body->restitution;
            glm::vec3 horizontal = glm::vec3(body->velocity.x, 0.0f, body->velocity.z);
            horizontal *= (1.0f - body->friction);
            body->velocity.x = horizontal.x;
            body->velocity.z = horizontal.z;
        }
        body->angularVelocity *= 0.99f;
    }
    checkCollisions();
}
