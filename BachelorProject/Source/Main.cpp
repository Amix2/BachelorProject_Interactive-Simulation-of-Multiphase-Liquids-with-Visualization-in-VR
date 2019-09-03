
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <shaders/ComputeShader.h>
#include <dataStructures/GpuResources.h>
#include <particleObjects/ParticleObject.h>
#include <particleObjects/ParticleObjectCreator.h>
#include <particleObjects/ParticleObjectManager.h>
#include <shaders/ShaderCodeEditor.h>
#include <dataStructures/ParticleData.h>
#include <dataStructures/FluidType.h>
#include <Utils.h>
#include <Logger.h>
#include <Simulation.h>
#include <thread>
#include <fstream>
#include <ThreadManager.h>
#include <stdlib.h> 

#include <TEMP_graphic.h>

// put on if you want to see particles in Simple Visualizer, it will erase previous content
//#define LOG_TO_FILE

void printWorkGroupsCapabilities();

// init openGL functions
void initGL();

// init app elements
void initTools();

// atExit function
void cleanUp();

void funWithCompShader();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void TUTORIAL_framebuffer_size_callback(GLFWwindow* window, int width, int height);


GLFWwindow* window;

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


int main(int argc, char ** argv) {
	ShaderCodeEditor::init();
	atexit(cleanUp);

	#ifdef LOG_TO_FILE
	ParticleData::partFile.open("./Simple Visualizer/part.log");	  
	ParticleData::partFile << "const partString = \"";
	#endif
	loguru::g_preamble_date = false;
	loguru::init(argc, argv);
	loguru::add_file("log.log", loguru::Truncate, loguru::Verbosity_MAX);

	/* ----- Init window ----- */

	initGL();

	printWorkGroupsCapabilities();

/////////////////////////////////////////////////////////////////////////////////////

	TEMP_graphic::initGraphic(window);

	initTools();

	ParticleObject* prtQ = nullptr ;
	checkOpenGLErrors();
	//Simulation sim2;
	//sim2.runSimulation();
	funWithCompShader();

	ParticleData::printParticleObjectsData();
	return 0;


	Simulation sim;
	//sim.runSimulation();


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float* pp = (float*)GpuResources::openSSBO(BufferDatails.particlePositionsName);
		float* kk = (float*)GpuResources::openSSBO(BufferDatails.glassPositionsName);
		for (int i = 0; i < 100; i++) {
			sim.runSimulation();
			TEMP_graphic::showFrame(window);
		}
		pp[0] = 0.50;
		GpuResources::commitSSBO(BufferDatails.particlePositionsName);
		GpuResources::commitSSBO(BufferDatails.glassPositionsName);
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}

void initGL()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window name", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, TUTORIAL_framebuffer_size_callback);

	glewExperimental = GL_TRUE;

	if (glewInit() != 0) {
		exit(-3);
	}

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Started debugging");
}

void initTools()
{
	ParticleData::initArraysOnGPU();
	ParticleObjectCreator::init();
	ParticleObjectManager::init();
	FluidType::init();
}

void cleanUp()
{
	LOG_F(INFO, "Clean Up");
	for (std::vector<std::thread*>::const_reverse_iterator it = Threads::vecBegin(); it != Threads::vecEnd(); it++) {
		(*it)->detach();
		(*it)->~thread();
	}
#ifdef LOG_TO_FILE
	ParticleData::partFile << "\".split(\"|\")";
	ParticleData::partFile.close();
#endif
}

void funWithCompShader()
{
	Sleep(100);
	ParticleObjectDetais details{ 9, 2,2,2, 3,3,3 };
	ParticleObjectCreator::addObject(details);

	Sleep(100);
	Simulation sim;
	sim.runSimulation();

	//ParticleData::printNewAddedParticleData();

	Sleep(500);
	sim.runSimulation();
	sim.runSimulation();

	ParticleData::printParticleData();
	ParticleData::printGlassData();
	ParticleObjectDetais details2{ -1, 5,5,5, 2.5,0.5,3 };
	ParticleObjectCreator::addObject(details2);

	Sleep(500);
	sim.runSimulation();
	Sleep(500);
	sim.runSimulation();
	sim.runSimulation();
	sim.runSimulation();

	Sleep(100);
	ParticleObjectDetais details3{ -1, 12,12,12, 1,1,1 };
	ParticleObjectCreator::addObject(details3);

	Sleep(100);
	sim.runSimulation();
	Sleep(500);
	sim.runSimulation();
	sim.runSimulation();

	//ParticleData::printParticleData();
	//Sleep(100);
	//ParticleData::printToAddParticleData();

	//Sleep(1000);


	//sim.runSimulation();
	Sleep(100);
	ParticleData::printParticleData(-1);
	ParticleData::printGlassData();
	ParticleObjectManager::printObjects();
#ifdef LOG_TO_FILE
	ParticleData::logParticlePositions();
#endif
}

void printWorkGroupsCapabilities() {
	GLint64  val_array[3];
	GLint64  value;

	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &val_array[0]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &val_array[1]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &val_array[2]);

	printf("GL_MAX_COMPUTE_WORK_GROUP_COUNT:\n\tx:%I64u\n\ty:%I64u\n\tz:%I64u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &val_array[0]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &val_array[1]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &val_array[2]);

	printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE:\n\tx:%I64u\n\ty:%I64u\n\tz:%I64u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64v(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
	printf("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &value);
	printf("GL_MAX_SHADER_STORAGE_BLOCK_SIZE :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_TEXTURE_SIZE, &value);
	printf("GL_MAX_TEXTURE_SIZE :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
	printf("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS  :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &value);
	printf("GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS   :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
	printf("GL_MAX_UNIFORM_BUFFER_BINDINGS    :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &value);
	printf("GL_MAX_COMPUTE_UNIFORM_COMPONENTS    :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_UNIFORM_BLOCK_SIZE, &value);
	printf("GL_MAX_UNIFORM_BLOCK_SIZE   :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &value);
	printf("GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS     :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
	printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE   :\n\t%I64u\n", value);
	value = -1;

	checkOpenGLErrors();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void TUTORIAL_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}