// Constraint.h
#pragma once
#include "RigidBody.h"
class Constraint {
public:
    RigidBody* bodyA;
    RigidBody* bodyB;
    virtual void solve() = 0;
    virtual ~Constraint() {}
};
