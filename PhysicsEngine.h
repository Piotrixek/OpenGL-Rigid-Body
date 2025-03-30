// PhysicsEngine.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "RigidBody.h"
#include "Constraint.h"
#include "CollisionData.h"
#include <queue> // For broadphase collision detection

class PhysicsEngine {
public:
    std::vector<RigidBody*> bodies;
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    glm::vec3 windForce = glm::vec3(0.0f);
    float gravityMultiplier = 1.0f;
    float timeStep = 1.0f / 60.0f; // Default time step
    float restitution = 0.8f; // Coefficient of restitution for collisions
    float staticFriction = 0.5f;
    float dynamicFriction = 0.3f;

    // Body Management
    void addBody(RigidBody* body);
    void removeBody(RigidBody* body);
    void clearBodies();

    // Simulation Control
    void update(float dt);
    void setTimeStep(float dt);
    float getTimeStep() const;

    // Forces and Torques
    void applyForce(RigidBody* body, const glm::vec3& force);
    void applyForceAtPoint(RigidBody* body, const glm::vec3& force, const glm::vec3& point);
    void applyTorque(RigidBody* body, const glm::vec3& torque);

    // Collision Detection and Resolution
    void checkCollisions();
    void resolveCollision(RigidBody* bodyA, RigidBody* bodyB, const CollisionData& collision); // you have also need to define struct for CollisionData 

    // Global Parameters
    void setGravity(const glm::vec3& gravity);
    void setWindForce(const glm::vec3& wind);
    void setGravityMultiplier(float multiplier);
    void setRestitution(float restitution);
    void setStaticFriction(float staticFriction);
    void setDynamicFriction(float dynamicFriction);

    // Constraints (Joints, Springs, etc.)
    void addConstraint(Constraint* constraint); // we also need to write code for Constraint
    void removeConstraint(Constraint* constraint);
    void solveConstraints();
    std::vector<Constraint*> constraints;

    // Spatial Partitioning (Broadphase)
    void broadphaseCollision();
    std::vector<std::pair<RigidBody*, RigidBody*>> potentialCollisions; // Pairs of bodies that might collide

    // Advanced Features
    void integrateForces(RigidBody* body, float dt);
    void integrateVelocity(RigidBody* body, float dt);

    // Optional: Sleep/Wake Management
    void sleepBodies();
    void wakeBody(RigidBody* body);

};