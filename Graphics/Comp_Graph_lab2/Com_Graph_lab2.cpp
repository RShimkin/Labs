#include <iostream>

#include<GL/glew.h>
#include<gl/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "Camera.h"
#include <iostream>
#include <soil.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool moveLight1 = false, moveLight2 = true;
bool on1 = true, on2 = false;

glm::vec3 lightPos(1.5f, 0.6f, 2.4f);
glm::vec3 lightPos1(0.0f, -0.5f, -2.0f);
glm::vec3 objectPos(0.0f, 0.0f, 0.0f);
glm::vec3 object2Pos(0.0f, 0.0f, -1.0f);

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1100, 800, "Lab3 Shimkin", nullptr, 0);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("vsBasicLighting.txt", "fsBasicLighting.txt");
	Shader lightingShader1("vsBasicLighting.txt", "fsBasicLighting.txt");
	Shader lampShader("vsLamp.txt", "fsLamp.txt");
	Shader lampShader1("vsLamp.txt", "fsLamp.txt");

	float vertices[] = {
			 0.0f,  0.0f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f,
			 0.0f,  0.3f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			 0.0f,  0.0f,  0.0f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f,
			 -0.2f, -0.2f, -0.3f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			 -0.2f, -0.2f, -0.3f,  -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			 -0.2f,  0.5f, -0.3f,  -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f,  0.3f,  -1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			 -0.2f, -0.2f, -0.3f,  -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			 -0.2f, -0.2f,  0.3f,  -1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			 -0.2f,  0.5f,  0.3f,  -1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			 -0.2f,  0.5f,  0.3f,  0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			 -0.2f, -0.2f,  0.3f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.0f,  0.0f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f,  0.3f, 0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.0f,  0.3f,  0.0f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.0f,  0.0f,  0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.0f,  0.3f,  0.0f,  0.0f,  -0.5f, -1.0f,   0.0f, 1.0f,
			  0.3f,  0.3f,  0.0f,  0.0f,  -0.5f, -1.0f,  0.0f, 0.0f,
			  0.5f,  0.5f, -0.3f,  0.0f,  -0.5f, -1.0f,  1.0f, 1.0f,

			  0.0f,  0.3f,  0.0f,  0.0f, -0.5f, -1.0f,   0.0f, 1.0f,
			  -0.2f,  0.5f, -0.3f, 0.0f, -0.5f, -1.0f,  0.0f, 0.0f,
			  0.5f,  0.5f, -0.3f,  0.0f, -0.5f, -1.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f, -0.3f, 0.0f,  1.0f, 0.0f,   0.0f, 1.0f,
			  0.5f,  0.5f, -0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
			  0.5f,  0.5f,  0.3f,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f,

			  -0.2f,  0.5f, -0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			  -0.2f,  0.5f,  0.3f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
			  0.5f,  0.5f,  0.3f,   0.0f,  1.0f, 0.0f,  1.0f, 1.0f,

			  0.5f,  0.5f,  0.3f,   0.0f,  -0.5f, 1.0f,   0.0f, 1.0f,
			  -0.2f,  0.5f,  0.3f,  0.0f,  -0.5f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.3f,  0.0f,   0.0f,  -0.5f, 1.0f,  1.0f, 1.0f,

			   0.5f,  0.5f,  0.3f, 0.0f,  -0.5f, 1.0f,   0.0f, 1.0f,
			  0.3f,  0.3f,  0.0f,  0.0f,  -0.5f, 1.0f,  0.0f, 0.0f,
			  0.0f,  0.3f,  0.0f,  0.0f,  -0.5f, 1.0f,  1.0f, 1.0f,

			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, -1.0f,   0.0f, 1.0f,
			  0.3f,  0.0f,  0.0f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			  0.5f, -0.2f, -0.3f,  -0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, -1.0f,   0.0f, 1.0f,
			  0.5f,  0.5f, -0.3f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f,
			  0.5f, -0.2f, -0.3f,  -0.5f,  0.0f, -1.0f,  1.0f, 1.0f,

			  0.5f,  0.5f, -0.3f,  1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			  0.5f, -0.2f, -0.3f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			  0.5f, -0.2f,  0.3f,  1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			  0.5f,  0.5f, -0.3f,  1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
			  0.5f,  0.5f,  0.3f,  1.0f,  0.0f, 0.0f,  0.0f, 0.0f,
			  0.5f, -0.2f,  0.3f,  1.0f,  0.0f, 0.0f,  1.0f, 1.0f,

			  0.5f, -0.2f,  0.3f,  -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.5f,  0.5f,  0.3f,  -0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.5f, -0.2f,  0.3f,  -0.5f,  0.0f, 1.0f,   0.0f, 1.0f,
			  0.3f,  0.0f,  0.0f,  -0.5f,  0.0f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.3f,  0.0f,  -0.5f,  0.0f, 1.0f,  1.0f, 1.0f,

			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,   0.0f, 1.0f,
			  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  0.5f, -1.0f,  1.0f, 1.0f,

			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, -1.0f,   0.0f, 1.0f,
			  0.5f, -0.2f, -0.3f,  0.0f,  0.5f, -1.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  0.5f, -1.0f,  1.0f, 1.0f,

			  0.5f, -0.2f, -0.3f,  0.0f,  -1.0f, 0.0f,   0.0f, 1.0f,
			  -0.2f, -0.2f, -0.3f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f,  0.3f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,

			  0.5f, -0.2f, -0.3f,  0.0f,  -1.0f, 0.0f,   0.0f, 1.0f,
			  0.5f, -0.2f,  0.3f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f,
			  -0.2f, -0.2f,  0.3f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,

			  -0.2f, -0.2f,  0.3f, 0.0f,  0.5f, 1.0f,  0.0f, 1.0f,
			  0.5f, -0.2f,  0.3f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  1.0f, 1.0f,

			  -0.2f, -0.2f,  0.3f, 0.0f,  0.5f, 1.0f,  0.0f, 1.0f,
			  0.0f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f,
			  0.3f,  0.0f,  0.0f,  0.0f,  0.5f, 1.0f,  1.0f, 1.0f
	};

	float vertices2[] = {
		0.6f, -0.6f, -0.1f,   0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // н п д
		0.6f, 0.6f, -0.1f,	  0.0f, 0.0f, -1.0f,    0.0f, 0.0f, // н п б
		-0.6f, 0.6f, -0.1f,   0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // н л б

		-0.6f, 0.6f, -0.1f,   0.0f, 0.0f, -1.0f,    1.0f, 0.0f, // н л б
		-0.6f, -0.6f, -0.1f,  0.0f, 0.0f, -1.0f,    1.0f, 1.0f, // н л д
		0.6f, -0.6f, -0.1f,   0.0f, 0.0f, -1.0f,    0.0f, 1.0f, // н п д

		0.6f, -0.6f, -0.1f,   0.24f, 0.0f, 0.12f,    0.0f, 1.0f, // н п д
		0.6f, 0.6f, -0.1f,	  0.24f, 0.0f, 0.12f,    0.0f, 0.0f, // н п б
		0.5f, -0.4f, 0.1f,    0.24f, 0.0f, 0.12f,    1.0f, 1.0f, // 1

		0.6f, 0.6f, -0.1f,	  0.0f, 0.24f, 0.12f,    0.0f, 0.0f, // н п б
		-0.6f, 0.6f, -0.1f,   0.0f, 0.24f, 0.12f,    1.0f, 1.0f, // н л б
		0.4f, 0.5f, 0.1f,     0.0f, 0.24f, 0.12f,    0.0f, 1.0f, // 2

		-0.6f, 0.6f, -0.1f,   -0.24f, 0.0f, 0.12f,    1.0f, 1.0f, // н л б
		-0.6f, -0.6f, -0.1f,  -0.24f, 0.0f, 0.12f,    0.0f, 0.0f, // н л д
		-0.5f, 0.4f, 0.1f,    -0.24f, 0.0f, 0.12f,    0.0f, 1.0f, // 3

		-0.6f, -0.6f, -0.1f,  0.0f, -0.24f, 0.12f,    0.0f, 0.0f, // н л д
		0.6f, -0.6f, -0.1f,   0.0f, -0.24f, 0.12f,    0.0f, 1.0f, // н п д
		-0.4f, -0.5f, 0.1f,   0.0f, -0.24f, 0.12f,    1.0f, 1.0f, // 4

		0.5f, -0.4f, 0.1f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // 1
		0.4f, 0.5f, 0.1f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // 2
		-0.5f, 0.4f, 0.1f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // 3

		0.5f, -0.4f, 0.1f,    0.18f, 0.02f, 0.19f,    1.0f, 1.0f, // 1
		0.4f, 0.5f, 0.1f,     0.18f, 0.02f, 0.19f,    0.0f, 0.0f, // 2
		0.6f, 0.6f, -0.1f,	  0.18f, 0.02f, 0.19f,    1.0f, 0.0f, // н п б

		0.4f, 0.5f, 0.1f,     -0.02f, 0.18f, 0.19f,    0.0f, 1.0f, // 2
		-0.5f, 0.4f, 0.1f,    -0.02f, 0.18f, 0.19f,    0.0f, 0.0f, // 3
		-0.6f, 0.6f, -0.1f,   -0.02f, 0.18f, 0.19f,    1.0f, 0.0f, // н л б

		0.5f, -0.4f, 0.1f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // 1
		-0.5f, 0.4f, 0.1f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // 3
		-0.4f, -0.5f, 0.1f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f, // 4

		-0.5f, 0.4f, 0.1f,    -0.18f, -0.02f, 0.19f,    1.0f, 1.0f, // 3
		-0.4f, -0.5f, 0.1f,   -0.18f, -0.02f, 0.19f,    0.0f, 0.0f, // 4
		-0.6f, -0.6f, -0.1f,  -0.18f, -0.02f, 0.19f,    1.0f, 0.0f, // н л д

		-0.4f, -0.5f, 0.1f,   0.02f, -0.18f, 0.19f,    0.0f, 1.0f, // 4
		0.5f, -0.4f, 0.1f,    0.02f, -0.18f, 0.19f,    1.0f, 0.0f, // 1
		0.6f, -0.6f, -0.1f,   0.02f, -0.18f, 0.19f,    0.0f, 0.0f, // н п д
	};

	unsigned int VBO, cubeVAO, VBO2, VAO2;
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture1;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	float shift = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 0.4f / 0.3f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 trans = glm::translate(model, glm::vec3(0.0f, 0.0f, -shift));
		model = glm::translate(trans, lightPos);
		if (moveLight1) {
			model = glm::translate(model, objectPos - lightPos);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, -(objectPos - lightPos));
		}
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		lampShader1.use();
		lampShader1.setMat4("projection", projection);
		lampShader1.setMat4("view", view);
		glm::mat4 model1 = glm::mat4(1.0f);
		glm::mat4 trans1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, -shift));
		model1 = glm::translate(trans1, lightPos1);
		if (moveLight2) {
			model1 = glm::translate(model1, objectPos - lightPos1);
			model1 = glm::rotate(model1, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model1 = glm::translate(model1, -(objectPos - lightPos1));
		}
		model1 = glm::scale(model1, glm::vec3(0.2f));
		lampShader1.setMat4("model", model1);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		lightingShader.use();
		lightingShader.setInt("texture1", 0);
		lightingShader.setBool("on1", on1);
		lightingShader.setBool("on2", on2);
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", glm::vec3(model[3]));
		lightingShader.setVec3("lightPos1", glm::vec3(model1[3]));
		lightingShader.setVec3("viewPos", camera.Position);

		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(trans, objectPos);
		lightingShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);

		model = glm::mat4(1.0f);
		model = glm::translate(trans, object2Pos);
		model = glm::scale(model, glm::vec3(2.0f));
		lightingShader.setMat4("model", model);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();

		shift += 0.01f;
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
