#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"

#include <iostream>
#include <vector>
#include <cmath>

const char *WINDOW_TITLE = "Hello World";
int WIDTH = 600;
int HEIGHT = 800;

std::vector<float> vertices;
std::vector<unsigned int> indices;

void polygon1() {
    vertices = {
        // positions       //colors
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 1.0f, 0.2f, 0.4f,
        0.0f, 0.0f, -1.0f, 1.0f, 0.2f, 0.4f,
    };

    indices = {
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
        8, 5, 4,
        8, 7, 4,
        8, 7, 6,
        8, 6, 5,
        9, 1, 2,
        9, 1, 0,
        9, 3, 2,
        9, 3, 0,
    };
}

void polygon2() {
    for (int i = 0; i < 6; i++) {
        float angle = glm::radians((float) i * (360.0f / 6.0f));
        vertices.push_back(glm::cos(angle));
        vertices.push_back(0.0f);
        vertices.push_back(glm::sin(angle));

        vertices.push_back(glm::sin(angle));
        vertices.push_back(glm::cos(angle));
        vertices.push_back(glm::sin(2 * angle));
    }
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    for (int i = 0; i < 6; i++) {
        indices.push_back(6);
        indices.push_back((i + 1) % 6);
        indices.push_back(i);

        indices.push_back(7);
        indices.push_back((i + 1) % 6);
        indices.push_back(i);
    }
}

void polygon3() {
    for (int i = 0; i < 11; i++) {
        float angle = glm::radians((float) i * (360.0f / 11.0f));
        vertices.push_back(glm::cos(angle));
        vertices.push_back(0.0f);
        vertices.push_back(glm::sin(angle));

        vertices.push_back(glm::sin(angle));
        vertices.push_back(glm::cos(angle));
        vertices.push_back(glm::sin(2 * angle));
    }
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    vertices.push_back(1.0f);
    vertices.push_back(1.0f);
    vertices.push_back(1.0f);

    for (int i = 0; i < 11; i++) {
        indices.push_back(11);
        indices.push_back((i + 1) % 11);
        indices.push_back(i);

        indices.push_back(12);
        indices.push_back((i + 1) % 11);
        indices.push_back(i);
    }
}

int cameraOn[3] = {0, 0, 0};
int objectOn[3] = {0, 0, 0};
int rotateObj = 0;
int rotateCam = 0;

float cameraSenstivity = 0.01f;
float objectSenstivity = 0.01f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 orientation1 = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 orientation2 = glm::vec3(-3.0f, 1.5f, 3.0f);
glm::vec3 orientation3 = glm::vec3(3.0f, 1.5f, 3.0f);

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
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    /* Set element buffer */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
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
    model = glm::translate(model, objectPos);
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
    bool oppositeCamera = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        oppositeCamera = true;
    }

    /* Reset camera movement */
    for (auto &it: cameraOn) {
        it = 0;
    }

    /* Camera movements */
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        cameraOn[0] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        cameraOn[1] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        cameraOn[2] = oppositeCamera ? -1 : 1;
    }

    /* Reset object movements */
    for (auto &it: objectOn) {
        it = 0;
    }

    /* Object movements */
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        objectOn[0] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        objectOn[1] = oppositeCamera ? -1 : 1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        objectOn[2] = oppositeCamera ? -1 : 1;
    }

    /* Set orientation */
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        cameraPos = orientation1;
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        cameraPos = orientation2;
    } else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        cameraPos = orientation3;
    }

    rotateObj = 0;
    rotateCam = 0;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (oppositeCamera) {
            rotateCam = 1;
        } else {
            rotateObj = 1;
        }
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
    transformation = glm::translate(transformation,
                                    objectSenstivity * glm::vec3(objectOn[0],
                                                                 objectOn[1],
                                                                 objectOn[2]));
    objectPos = glm::vec3(transformation * glm::vec4(objectPos, 1.0f));
}

void setPolygon() {
    std::cout << "Enter which polygon to display (1 - 3)\n";

    int polygonNum;
    std::cin >> polygonNum;

    switch (polygonNum) {
        case 1:polygon1();
            break;
        case 2:polygon2();
            break;
        case 3:polygon3();
            break;
        default:std::cout << "Invalid choice\n";
            std::exit(1);
    }
}

int main() {
    setPolygon();

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
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        /* Check and call events and swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    handleExit();
}