// SimulationState.cpp
#include "SimulationState.h"
#include <glm/glm.hpp>
#include <fstream>
void resetSimulation(PhysicsEngine& engine, RigidBody& body1, RigidBody& body2) {
    body1.position = glm::vec3(-1.0f, 5.0f, 0.0f);
    body1.velocity = glm::vec3(0.0f);
    body1.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    body2.position = glm::vec3(1.0f, 8.0f, 0.0f);
    body2.velocity = glm::vec3(0.0f);
    body2.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}
void saveSimulationState(const PhysicsEngine& engine, const std::string& filename) {
    std::ofstream file(filename);
    for (auto body : engine.bodies) {
        file << body->position.x << " " << body->position.y << " " << body->position.z << "\n";
        file << body->velocity.x << " " << body->velocity.y << " " << body->velocity.z << "\n";
    }
    file.close();
}
void loadSimulationState(PhysicsEngine& engine, const std::string& filename) {
    std::ifstream file(filename);
    for (auto body : engine.bodies) {
        file >> body->position.x >> body->position.y >> body->position.z;
        file >> body->velocity.x >> body->velocity.y >> body->velocity.z;
    }
    file.close();
}
