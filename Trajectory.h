// Trajectory.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
class Trajectory {
public:
    std::vector<glm::vec3> points;
    void addPoint(const glm::vec3& point);
    void draw();
};
