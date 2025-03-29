// Plane.cpp
#include "Plane.h"
unsigned int planeVAO = 0, planeVBO = 0;
void initPlane() {
    float planeVertices[] = {
         50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f, -50.0f,
         50.0f, 0.0f,  50.0f,
        -50.0f, 0.0f, -50.0f,
         50.0f, 0.0f, -50.0f,
    };
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
void drawPlane() {
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
