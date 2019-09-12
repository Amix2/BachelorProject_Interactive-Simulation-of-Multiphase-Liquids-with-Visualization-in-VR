#include "TEMP_graphic.h"
#include <Utils.h>

void TUTORIAL_framebuffer_size_callback(GLFWwindow* window, int width, int height);
void TUTORIAL_processInput(GLFWwindow* window);
void TUTORIAL_InitShaders();

const char* vertexShaderSource = "#version 430 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 430 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"layout(std430, binding = 1) buffer positionsBuf\n"
"{\n"
"	float fluidPositions[3 * 10];\n"
"};\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"   FragColor.x = fluidPositions[0];\n"
"}\n\0";
float color = 0.0;
void TEMP_graphic::initGraphic(GLFWwindow* window)
{

	// create shaders
	TUTORIAL_InitShaders();

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);


	// bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
	// just bind it beforehand before rendering the respective triangle; this is another approach.
	glBindVertexArray(VAO);

	glUseProgram(shaderProgram);

	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 1.0f, 0.5f, color, 1.0f);

}
float greenValue = 0;
void TEMP_graphic::showFrame(GLFWwindow* window)
{
	long tStart = getTime();
	//glBindBuffer(0);
	TUTORIAL_processInput(window);

	// render
// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// be sure to activate the shader before any calls to glUniform
	glUseProgram(shaderProgram);

	// update shader uniform
	float timeValue = glfwGetTime();
	greenValue += 0.4;
	if (greenValue > 1) greenValue =0.5;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	// render the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// ------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
	long tEnd = getTime();
	//LOG_F(ERROR, "DRAW time: %d", tEnd - tStart);
	checkOpenGLErrors();
}

void TUTORIAL_processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void TUTORIAL_InitShaders()
{
	// build and compile our shader program
// ------------------------------------
// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	TEMP_graphic::shaderProgram = glCreateProgram();
	glAttachShader(TEMP_graphic::shaderProgram, vertexShader);
	glAttachShader(TEMP_graphic::shaderProgram, fragmentShader);
	glLinkProgram(TEMP_graphic::shaderProgram);
	// check for linking errors
	glGetProgramiv(TEMP_graphic::shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(TEMP_graphic::shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
