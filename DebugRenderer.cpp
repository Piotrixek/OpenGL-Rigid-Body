// DebugRenderer.cpp
#include "DebugRenderer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void drawDebugBox(const RigidBody& body) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, body.position);
    model = glm::scale(model, glm::vec3(body.radius * 2.0f));
    static unsigned int debugVAO = 0, debugVBO = 0, debugEBO = 0;
    if (debugVAO == 0) {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, // 0
             0.5f, -0.5f, -0.5f, // 1
             0.5f,  0.5f, -0.5f, // 2
            -0.5f,  0.5f, -0.5f, // 3
            -0.5f, -0.5f,  0.5f, // 4
             0.5f, -0.5f,  0.5f, // 5
             0.5f,  0.5f,  0.5f, // 6
            -0.5f,  0.5f,  0.5f  // 7
        };
        unsigned int indices[] = {
            0, 1, 1, 2, 2, 3, 3, 0, // back face
            4, 5, 5, 6, 6, 7, 7, 4, // front face
            0, 4, 1, 5, 2, 6, 3, 7  // side edges
        };
        glGenVertexArrays(1, &debugVAO);
        glGenBuffers(1, &debugVBO);
        glGenBuffers(1, &debugEBO);
        glBindVertexArray(debugVAO);
        glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, debugEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(debugVAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
