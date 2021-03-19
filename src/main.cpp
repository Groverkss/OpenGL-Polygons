#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"

#include <iostream>
#include <cmath>

const char *WINDOW_TITLE = "Hello World";
int WIDTH = 600;
int HEIGHT = 800;

float vertices[] = {
    // positions       //colors
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
};

unsigned int indices[] = {
    0, 1, 2,
    0, 3, 2,
    0, 1, 5,
    0, 4, 5,
    4, 7, 6,
    6, 5, 4,
    3, 7, 6,
    3, 2, 6,
    6, 5, 2,
    2, 5, 1,
    7, 4, 3,
    3, 4, 0,
};

int cameraOn[3] = {0, 0, 0};
float cameraSenstivity = 0.01f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window,
                               int width,
                               int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

GLFWwindow *initWindow(int width, int height, const char *title) {

    /* Set Window hints */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Initialize GLFW window */
    auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    /* Initialize Glad */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    /* Initial dimensions to OpenGL */
    glViewport(0, 0, width, height);

    /* Resize window on window size change */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void handleExit() {
    glfwTerminate();
}

Shader createShaders() {
    auto shaders = Shader("src/vertexShader.glsl",
                          "src/fragmentShader.glsl");
    return shaders;
}

unsigned int getVAO() {
    /* Create VAO */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Set vertex buffer */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Set element buffer */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);

    /* Set vertex pointers */
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void *) 0);
    glEnableVertexAttribArray(0);

    /* Set vertex colors */
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

void setTransformations(Shader shaders) {
    /* Set model matrix */
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f,
                                        0.5f));

    /* Set camera view matrix */
    glm::mat4 view = glm::lookAt(cameraPos, objectPos, cameraUp);

    /* Set projection matrix */
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 16.0f / 9.0f,
                         0.1f,
                         100.f);

    shaders.setMat4("model", model);
    shaders.setMat4("view", view);
    shaders.setMat4("projection", projection);
}

void processInput(GLFWwindow *window) {
    /* Camera movements */
    bool oppositeCamera = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        oppositeCamera = true;
    }

    /* Reset camera movement */
    for (auto &it: cameraOn) {
        it = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        cameraOn[0] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        cameraOn[1] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        cameraOn[2] = oppositeCamera ? -1 : 1;
    }
}

void moveCamera() {
    glm::mat4 transformation = glm::mat4(1.0f);
    transformation = glm::translate(transformation,
                                    cameraSenstivity * glm::vec3(cameraOn[0],
                                                                 cameraOn[1],
                                                                 cameraOn[2]));
    cameraPos = glm::vec3(transformation * glm::vec4(cameraPos, 1.0f));
}

void moveObject() {
    glm::mat4 transformation = glm::mat4(1.0f);
    cameraPos = glm::vec3(transformation * glm::vec4(cameraPos, 1.0f));
}

int main() {
    auto window = initWindow(WIDTH, HEIGHT, WINDOW_TITLE);

    auto shaders = createShaders();
    auto VAO = getVAO();

    /* While window is not closed */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /* Move camera based on inputs recieved */
        moveCamera();
        moveObject();

        /* Set window background to Green */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /* Enable shaders */
        shaders.use();
        setTransformations(shaders);

        /* Bind Vertices */
        glBindVertexArray(VAO);

        /* Draw Vertices */
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        /* Check and call events and swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    handleExit();
}