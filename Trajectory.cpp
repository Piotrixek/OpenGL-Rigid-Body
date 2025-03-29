// Trajectory.cpp
#include "Trajectory.h"
#include <glad/glad.h>

void Trajectory::addPoint(const glm::vec3& point) {
    points.push_back(point);
    if (points.size() > 1000) points.erase(points.begin());
}

void Trajectory::draw() {
    if (points.empty()) return;
    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(points.size()));
    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
