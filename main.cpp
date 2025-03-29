// main.cpp
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Camera.h"
#include "Logger.h"
#include "Shader.h"
#include "Callbacks.h"
#include "Globals.h"
#include "PhysicsEngine.h"
#include "RigidBody.h"
#include "Cube.h"
#include "Plane.h"
#include "ShadowMap.h"
#include "ParticleSystem.h"
#include "Trajectory.h"
#include "CameraFollow.h"
#include "SimulationState.h"
#include "FPSCounter.h"
#include "DebugRenderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Simulation", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unsigned int skyVAO, skyVBO;
    float skyVertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int skyShader = loadShader("shaders/sky.vert", "shaders/sky.frag");
    if (!skyShader) {
        std::cerr << "Critical sky shader load error!\n";
        glfwTerminate();
        return -1;
    }
    unsigned int rigidBodyShader = loadShader("shaders/rigidbody.vert", "shaders/rigidbody.frag");
    if (!rigidBodyShader) {
        std::cerr << "Critical rigid body shader load error!\n";
        glfwTerminate();
        return -1;
    }

    initCube();
    initPlane();

    ShadowMap shadowMap;
    shadowMap.init();
    ParticleSystem particleSystem(100);
    Trajectory trajectory;
    FPSCounter fpsCounter;

    PhysicsEngine physicsEngine;
    RigidBody body1(glm::vec3(-1.0f, 5.0f, 0.0f), 1.0f, 0.5f);
    RigidBody body2(glm::vec3(1.0f, 8.0f, 0.0f), 1.0f, 0.5f);
    physicsEngine.addBody(&body1);
    physicsEngine.addBody(&body2);

    followTarget = &body1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    bool windEnabled = true, showPlane = true, cameraFollowToggle = false, debugMode = false;
    float windMultiplier = 1.0f, simulationSpeed = 1.0f;
    float cubeColor[3] = { 0.8f, 0.3f, 0.3f };
    float groundColor[3] = { 0.3f, 0.8f, 0.3f };
    float lightPosArr[3] = { 5.0f, 10.0f, 5.0f };
    float lightColorArr[3] = { 1.0f, 1.0f, 1.0f };
    float gravityMultiplier = 1.0f, camSpeed = camera.speed;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        fpsCounter.update(deltaTime);
        particleSystem.update(deltaTime);
        if (cameraFollowToggle) { updateCameraFollow(); }
        trajectory.addPoint(body1.position);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Controls");
        ImGui::Text("RigidBody 1 Pos: (%.2f, %.2f, %.2f)", body1.position.x, body1.position.y, body1.position.z);
        ImGui::Text("RigidBody 2 Pos: (%.2f, %.2f, %.2f)", body2.position.x, body2.position.y, body2.position.z);
        ImGui::SliderFloat("Gravity", &gravityMultiplier, 0.0f, 5.0f);
        physicsEngine.gravityMultiplier = gravityMultiplier;
        ImGui::Checkbox("Wind Enabled", &windEnabled);
        ImGui::SliderFloat("Wind Multiplier", &windMultiplier, 0.0f, 20.0f);
        physicsEngine.windForce = windEnabled ? glm::vec3(windMultiplier, 0.0f, 0.0f) : glm::vec3(0.0f);
        ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.1f, 5.0f);
        ImGui::Checkbox("Show Ground Plane", &showPlane);
        ImGui::ColorEdit3("Cube Color", cubeColor);
        ImGui::ColorEdit3("Ground Color", groundColor);
        ImGui::SliderFloat3("Light Position", lightPosArr, -20.0f, 20.0f);
        ImGui::ColorEdit3("Light Color", lightColorArr);
        ImGui::SliderFloat("Camera Speed", &camSpeed, 1.0f, 20.0f);
        camera.speed = camSpeed;
        ImGui::Checkbox("Camera Follow", &cameraFollowToggle);
        ImGui::Checkbox("Debug Mode", &debugMode);
        if (ImGui::Button("Reset Simulation")) {
            resetSimulation(physicsEngine, body1, body2);
        }
        if (ImGui::Button("Save Simulation")) {
            saveSimulationState(physicsEngine, "sim_state.txt");
        }
        if (ImGui::Button("Load Simulation")) {
            loadSimulationState(physicsEngine, "sim_state.txt");
        }
        ImGui::Text("FPS: %.1f", fpsCounter.fps);
        ImGui::End();

        physicsEngine.update(deltaTime * simulationSpeed);


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = camera.getProjectionMatrix(1280.0f / 720.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);


        glDisable(GL_DEPTH_TEST);
        glUseProgram(skyShader);
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glEnable(GL_DEPTH_TEST);


        glUseProgram(rigidBodyShader);
        int modelLoc = glGetUniformLocation(rigidBodyShader, "model");
        int viewLoc = glGetUniformLocation(rigidBodyShader, "view");
        int projLoc = glGetUniformLocation(rigidBodyShader, "projection");
        int objColorLoc = glGetUniformLocation(rigidBodyShader, "objectColor");
        int lightPosLoc = glGetUniformLocation(rigidBodyShader, "lightPos");
        int lightColorLoc = glGetUniformLocation(rigidBodyShader, "lightColor");
        int viewPosLoc = glGetUniformLocation(rigidBodyShader, "viewPos");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
        glUniform3f(lightPosLoc, lightPosArr[0], lightPosArr[1], lightPosArr[2]);
        glUniform3f(lightColorLoc, lightColorArr[0], lightColorArr[1], lightColorArr[2]);
        glUniform3f(viewPosLoc, camera.position.x, camera.position.y, camera.position.z);

        if (showPlane) {
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
            glUniform3f(objColorLoc, groundColor[0], groundColor[1], groundColor[2]);
            drawPlane();
        }
        for (auto body : physicsEngine.bodies) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, body->position);
            model = model * glm::mat4_cast(body->orientation);
            model = glm::scale(model, glm::vec3(body->radius * 2.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
            glUniform3f(objColorLoc, cubeColor[0], cubeColor[1], cubeColor[2]);
            drawCube();
            if (debugMode)
                drawDebugBox(*body);
        }
        particleSystem.draw();
        trajectory.draw();

        logger.draw("Application Log");
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteVertexArrays(1, &skyVAO);
    glDeleteBuffers(1, &skyVBO);
    glfwTerminate();
    return 0;
}
