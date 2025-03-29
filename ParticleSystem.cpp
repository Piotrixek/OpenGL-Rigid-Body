// ParticleSystem.cpp
#include "ParticleSystem.h"
#include <glad/glad.h>
#include <cstdlib>
ParticleSystem::ParticleSystem(unsigned int count) {
    particles.resize(count);
    for (auto& p : particles) {
        p.position = glm::vec3(0.0f);
        p.velocity = glm::vec3(((rand() % 100) / 100.0f - 0.5f) * 2.0f, ((rand() % 100) / 100.0f), ((rand() % 100) / 100.0f - 0.5f) * 2.0f);
        p.life = (rand() % 100) / 100.0f;
    }
    init();
}
void ParticleSystem::init() {
    float quadVertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);
    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void ParticleSystem::update(float dt) {
    for (auto& p : particles) {
        p.life -= dt;
        if (p.life <= 0.0f) {
            p.position = glm::vec3(0.0f);
            p.velocity = glm::vec3(((rand() % 100) / 100.0f - 0.5f) * 2.0f, ((rand() % 100) / 100.0f), ((rand() % 100) / 100.0f - 0.5f) * 2.0f);
            p.life = 1.0f;
        }
        p.position += p.velocity * dt;
    }
}
void ParticleSystem::draw() {
    glBindVertexArray(particleVAO);
    glDrawArrays(GL_POINTS, 0, particles.size());
    glBindVertexArray(0);
}
