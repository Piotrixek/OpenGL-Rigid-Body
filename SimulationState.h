// SimulationState.h
#pragma once
#include "RigidBody.h"
#include "PhysicsEngine.h"
#include <string>
void resetSimulation(PhysicsEngine& engine, RigidBody& body1, RigidBody& body2);
void saveSimulationState(const PhysicsEngine& engine, const std::string& filename);
void loadSimulationState(PhysicsEngine& engine, const std::string& filename);
