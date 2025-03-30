// PhysicsEngine.cpp
#include "PhysicsEngine.h"
#include "CollisionData.h"
#include "Constraint.h"
#include <algorithm>
#include <cmath>

void PhysicsEngine::addBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsEngine::removeBody(RigidBody* body) {
    bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
}

void PhysicsEngine::clearBodies() {
    bodies.clear();
}

void PhysicsEngine::setTimeStep(float dt) {
    timeStep = dt;
}

float PhysicsEngine::getTimeStep() const {
    return timeStep;
}

void PhysicsEngine::applyForce(RigidBody* body, const glm::vec3& force) {
    body->applyForce(force);
}

void PhysicsEngine::applyForceAtPoint(RigidBody* body, const glm::vec3& force, const glm::vec3& point) {
    body->applyForce(force);
    glm::vec3 r = point - body->position;
    body->applyTorque(glm::cross(r, force));
}

void PhysicsEngine::applyTorque(RigidBody* body, const glm::vec3& torque) {
    body->applyTorque(torque);
}

// Not used since integration is handled inside RigidBody::update
void PhysicsEngine::integrateForces(RigidBody* body, float dt) { }
void PhysicsEngine::integrateVelocity(RigidBody* body, float dt) { }

void PhysicsEngine::checkCollisions() {
    potentialCollisions.clear();
    // Broadphase: simple O(n^2) check
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            RigidBody* a = bodies[i];
            RigidBody* b = bodies[j];
            glm::vec3 diff = b->position - a->position;
            float dist = glm::length(diff);
            if (dist < (a->radius + b->radius)) {
                potentialCollisions.push_back(std::make_pair(a, b));
                CollisionData collision;
                collision.bodyA = a;
                collision.bodyB = b;
                collision.contactNormal = (dist > 0.0f) ? glm::normalize(diff) : glm::vec3(0.0f, 1.0f, 0.0f);
                collision.penetrationDepth = (a->radius + b->radius) - dist;
                collision.contactPoint = a->position + collision.contactNormal * a->radius;
                resolveCollision(a, b, collision);
            }
        }
    }
}

void PhysicsEngine::resolveCollision(RigidBody* bodyA, RigidBody* bodyB, const CollisionData& collision) {
    glm::vec3 relativeVelocity = bodyB->velocity - bodyA->velocity;
    float velAlongNormal = glm::dot(relativeVelocity, collision.contactNormal);
    if (velAlongNormal > 0.0f)
        return;
    float e = restitution; // using global restitution value
    float j = -(1.0f + e) * velAlongNormal;
    j /= (1.0f / bodyA->mass + 1.0f / bodyB->mass);
    glm::vec3 impulse = j * collision.contactNormal;
    bodyA->velocity -= impulse / bodyA->mass;
    bodyB->velocity += impulse / bodyB->mass;
    // Positional correction
    const float percent = 0.2f;
    glm::vec3 correction = percent * collision.penetrationDepth * collision.contactNormal;
    bodyA->position -= correction / bodyA->mass;
    bodyB->position += correction / bodyB->mass;
}

void PhysicsEngine::setGravity(const glm::vec3& g) {
    gravity = g;
}

void PhysicsEngine::setWindForce(const glm::vec3& wind) {
    windForce = wind;
}

void PhysicsEngine::setGravityMultiplier(float multiplier) {
    gravityMultiplier = multiplier;
}

void PhysicsEngine::setRestitution(float r) {
    restitution = r;
}

void PhysicsEngine::setStaticFriction(float sFriction) {
    staticFriction = sFriction;
}

void PhysicsEngine::setDynamicFriction(float dFriction) {
    dynamicFriction = dFriction;
}

void PhysicsEngine::addConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

void PhysicsEngine::removeConstraint(Constraint* constraint) {
    constraints.erase(std::remove(constraints.begin(), constraints.end(), constraint), constraints.end());
}

void PhysicsEngine::solveConstraints() {
    for (auto constraint : constraints) {
        constraint->solve();
    }
}

void PhysicsEngine::broadphaseCollision() {
    potentialCollisions.clear();
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            RigidBody* a = bodies[i];
            RigidBody* b = bodies[j];
            float dist = glm::length(b->position - a->position);
            if (dist < (a->radius + b->radius))
                potentialCollisions.push_back(std::make_pair(a, b));
        }
    }
}

void PhysicsEngine::sleepBodies() {
    for (auto body : bodies) {
        if (glm::length(body->velocity) < 0.01f) {
            // Mark as sleeping (not implemented in RigidBody)
        }
    }
}

void PhysicsEngine::wakeBody(RigidBody* body) {
    // Mark body as awake (not implemented in RigidBody)
}

void PhysicsEngine::update(float dt) {
    // Apply global forces to each body
    for (auto body : bodies) {
        body->applyForce(gravity * gravityMultiplier * body->mass);
        body->applyForce(windForce);
    }
    // Let each body integrate forces and update its state
    for (auto body : bodies) {
        body->update(dt);
        // Collision with the ground plane at y = 0
        if (body->position.y - body->radius < 0.0f) {
            float penetration = 0.0f - (body->position.y - body->radius);
            body->position.y += penetration;
            if (body->velocity.y < 0)
                body->velocity.y = -body->velocity.y * restitution;
            body->velocity.x *= (1.0f - staticFriction);
            body->velocity.z *= (1.0f - staticFriction);
        }
    }
    checkCollisions();
    solveConstraints();
    sleepBodies();
}
