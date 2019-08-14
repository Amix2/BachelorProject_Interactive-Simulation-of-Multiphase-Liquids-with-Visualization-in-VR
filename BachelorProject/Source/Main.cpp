
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
#include <dataStructures/ParticleData.h>
#include <particleObjects/ParticleObjectCreator.h>
#include <Utils.h>
#include <Logger.h>
#include <Simulation.h>
#include <thread>

void startSimulation();

void printWorkGroupsCapabilities();

int main(int argc, char ** argv) {
	loguru::g_preamble_date = false;
	loguru::init(argc, argv);

	/* ----- Init window ----- */
	GLFWwindow * window;
	if (!glfwInit()) {
		exit(-1);
	}

	window = glfwCreateWindow(640, 480, "window name", NULL, NULL);

	if (!window) {
		exit(-2);
	}

	glfwMakeContextCurrent(window);


	if (glewInit() != 0) {
		exit(-3);
	}


/* ----- Compute Shader ----- */

	printWorkGroupsCapabilities();

	//std::thread simulationThread(startSimulation);
	//simulationThread.join();


	ParticleData::initArraysOnGPU();


	ParticleObjectCreator::init();
	ParticleObjectDetais details{9, 0,0,0, 1,1,1};
	ParticleObjectCreator::addObject(details);

	Sleep(1000);
	Simulation sim;
	sim.runSimulation();

	//ParticleData::printNewAddedParticleData();
	ParticleData::printParticleData();

	//Sleep(1000);

	//Sleep(1000);

	//ParticleData::printNewAddedParticleData();
	//ParticleData::printParticleData();
	//Sleep(1000);

	//ParticleObjectDetais details2{ 8, 2,2,2, 4,4,4 };
	//ParticleObjectCreator::addObject(details2);
	//ParticleData::printNewAddedParticleData();
	//Sleep(1000);
	//sim.runSimulation();
	//Sleep(1000);
	//ParticleData::printParticleData();

	return 0;
}

void startSimulation() {
	//loguru::set_thread_name("simulation");
	Simulation sim;
	sim.runSimulation();
}


void printWorkGroupsCapabilities() {
	GLint64  val_array[3];
	GLint64  value;

	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &val_array[0]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &val_array[1]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &val_array[2]);

	printf("GL_MAX_COMPUTE_WORK_GROUP_COUNT:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &val_array[0]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &val_array[1]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &val_array[2]);

	printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE:\n\tx:%u\n\ty:%u\n\tz:%u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64v(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
	printf("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &value);
	printf("GL_MAX_SHADER_STORAGE_BLOCK_SIZE :\n\t%u\n", value);

	glGetInteger64v(GL_SHADER_STORAGE_BUFFER_SIZE, &value);
	printf("GL_SHADER_STORAGE_BUFFER_SIZE :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_TEXTURE_SIZE, &value);
	printf("GL_MAX_TEXTURE_SIZE :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
	printf("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS  :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &value);
	printf("GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS   :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
	printf("GL_MAX_UNIFORM_BUFFER_BINDINGS    :\n\t%u\n", value);

	glGetInteger64v(GL_UNIFORM_BUFFER_SIZE, &value);
	printf("GL_UNIFORM_BUFFER_SIZE   :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &value);
	printf("GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS     :\n\t%u\n", value);

	glGetInteger64v(GL_ATOMIC_COUNTER_BUFFER_SIZE, &value);
	printf("GL_ATOMIC_COUNTER_BUFFER_SIZE   :\n\t%u\n", value);

	glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
	printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE   :\n\t%u\n", value);

	checkOpenGLErrors();
}


