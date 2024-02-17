#include <GL/glew.h>
#include<GL/glut.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using glm::mat4;
using glm::vec3;
using glm::rotate;
int main()
{



	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/* tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */

	// close GL context and any other GLFW resources
	//glfwTerminate();
	float points[] =
	{
0.0f,  0.5f,  0.0f,
0.5f, -0.5f,  0.0f,
-0.5f, -0.5f,  0.0f
	};
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"uniform mat4 model;"
		"void main() {"

		"  gl_Position = model* vec4(vp, 1.0);"

		"}";
	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main() {"
		"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
		"}";
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*   glfwWindowShouldClose(window) -проверяет в начале каждой итерации цикла, получил ли GLFW инструкцию к закрытию,
		 если так — то функция вернет true и игровой цикл перестанет работать, после чего мы сможем закрыть наше приложение.   */

	while (!glfwWindowShouldClose(window))
	{
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glm::mat4 model(1.0f);
		//движение с поворотом
		model = glm::translate(model, glm::vec3(0.3f, -0.3f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
		// статический поворот
		//model = glm::rotate(model, 30.0f, vec3(0.0f, 0.0f, 1.0f));



		GLuint location = glGetUniformLocation(shader_programme, "model");
		if (location >= 0)
		{

			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
			// для статического поворота
			//glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);
		}


		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*  glfwPollEvents() - проверяет были ли вызваны какие либо события (вроде ввода с клавиатуры или перемещение мыши)
					  и вызывает установленные функции (которые мы можем установить через функции обратного вызова (callback)*/

		glfwPollEvents();

		/*  glfwSwapBuffers(window) - заменяет цветовой буфер (большой буфер, содержащий значения цвета для каждого пикселя в GLFW окне),
					   который использовался для отрисовки во время текущей итерации и показывает результат на экране */

		glfwSwapBuffers(window);
	}

	return 0;
}