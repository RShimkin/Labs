#include <stdio.h>
#include "Shaders.h"
#include <soil.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::rotate;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

bool affine = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader("vertexShader.txt", "fragmentShader.txt");

    float vertices[] = {
        // координаты       // цвета
        -0.1f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,   0.2f, 0.0f,  //0
        -0.3f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.0f, 0.1f,  //1
        -0.3f,  0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.0f, 0.3f,  //2
        -0.1f,  0.2f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.2f, 0.4f,  //3
         0.1f,  0.2f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.8f, 0.4f,  //4
         0.3f,  0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  1.0f, 0.3f,  //5
         0.3f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  1.0f, 0.1f,  //6
         0.1f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.8f, 0.0f,  //7
        -0.25f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.0f, 0.0f,  //8
        -0.15f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.1f, 0.0f,  //9
        -0.25f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.0f, 0.3f,  //10
        -0.15f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.1f, 0.3f,  //11
        -0.15f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.1f, 0.4f,  //12
         0.15f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.4f, 0.4f,  //13
         0.15f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.4f, 0.3f,  //14
         0.25f, 0.4f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.5f, 0.3f,  //15
         0.15f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.4f, 0.0f,  //16
         0.25f, 0.1f, 0.0f,   0.0f, 0.0f, 1.0f,  0.4f, 0.5f, 0.6f,  0.5f, 0.0f   //17
    };

    float vtrian[] = {
        //0.9f, 1.1f, -0.1f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.3f, 0.4f,
        0.9f, 1.1f,  0.1f,  0.0f, 0.0f, 1.0f,   0.9f, 0.9f, 0.9f,   0.3f, 0.4f,
        //0.9f, -1.0f, -0.1f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,   0.5f, 0.6f,
        0.0f, -1.0f,  0.1f,  0.0f, 0.0f, 1.0f,  0.9f, 0.9f, 0.9f,   0.5f, 0.6f,
        //0.3f, 0.7f, -0.1f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f
        -0.4f, 0.7f,  0.1f,  0.0f, 0.0f, 1.0f,   0.9f, 0.9f, 0.9f,   0.0f, 0.0f
    };

    unsigned int indices[] = {
        8,9,10,
        10,9,11,
        10,11,12,
        11,12,13,
        13,11,14,
        14,13,15,
        15,14,16,
        16,15,17,
        0,1,2,
        2,0,3,
        4,5,6,
        6,4,7
    };

    //--------------------------------------------------
    unsigned int VBOs[2], VAOs[2];
    unsigned int EBO;
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glNormal3f(0, 0, 1);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtrian), vtrian, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glNormal3f(0, 0, 1);
    //----------------------------------------------------

    glBindVertexArray(0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* data = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float x = 0.55, y = 0.7, z = 0.5, xm = -1, ym = -1, zm = 1;
    float shift = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
      
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);

        if (affine) {
            //model = glm::translate(model, glm::vec3(0.3f, -0.3f, 0.0f));
            //model = glm::rotate(model, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
            //model = glm::rotate(model, (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, 30.0f, vec3(0.0f, 0.0f, 1.0f));
            //model = glm::scale(model, vec3(shift, shift, shift));
            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.4f));
            //model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.99));
            if (shift <= 1.1) shift += 0.001f;
        }

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6 * sizeof(indices), GL_UNSIGNED_INT, 0);

        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -shift));
        //if (shift > 0.2) 
            

        shader.use();

        float timeValue = glfwGetTime();
        GLuint location = glGetUniformLocation(shader.ID, "model");
        if (location >= 0)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
            if (affine) glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);
        }
        //if (x < 5 || x > 3) xm *= -1;
        //if (y < 3 || y > 2) ym *= -1;
        //if (z < 0.05 || z > 10) zm *= -1;
        //z += zm * 0.0008 * (abs(z - 0.04));

        //float pos[] = { x,y,z,1 };
        float pos[] = { 0.0f, 0.0f, 0.0f, 1 };
        glLightfv(GL_LIGHT0, GL_POSITION, pos);

        shader.setVec3("lightPos", glm::vec3(x, y, z));
        shader.setVec3("viewPos", glm::vec3(0, 0, 5));

        //glBindVertexArray(VAOs[1]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //glBindVertexArray(VAOs[0]);
        //glDrawElements(GL_TRIANGLES, 6 * sizeof(indices), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        affine = !affine;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {   
    glViewport(0, 0, width, height);
}