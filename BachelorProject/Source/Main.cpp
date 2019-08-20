
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
#include <particleObjects/ParticleObject.h>
#include <Utils.h>
#include <Logger.h>
#include <Simulation.h>
#include <thread>
#include <fstream>
#include <ThreadManager.h>

void startSimulation();

void printWorkGroupsCapabilities();


int main(int argc, char ** argv) {

	ParticleData::partFile.open("./Simple Visualizer/part.log");	 
	ParticleData::partFile << "const partString = \"";
	loguru::g_preamble_date = false;
	loguru::init(argc, argv);
	loguru::add_file("log.log", loguru::Truncate, loguru::Verbosity_MAX);

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
	ParticleObjectDetais details{9, 2,2,2, 3,3,3};
	ParticleObjectCreator::addObject(details);

	Sleep(100);
	Simulation sim;
	sim.runSimulation();

	//ParticleData::printNewAddedParticleData();

	Sleep(100);

	//ParticleObjectDetais details2{ -1, 5,5,5, 1.5,1,2.5 };
	//ParticleObjectCreator::addObject(details2);
	ParticleData::printParticleData();
	float* pp = (float*)GpuResources::openSSBO(BufferDatails.particlePositionsName);
	Sleep(100);
	//ParticleData::printToAddParticleData();

	Sleep(1000);

	sim.runSimulation();

	pp[0] = -1;
	GpuResources::commitSSBO(BufferDatails.particlePositionsName);
	sim.runSimulation();
	Sleep(100);
	ParticleData::printParticleData();
	//ParticleData::printGlassData(1);
	//ParticleData::logParticlePositions();

	for (std::vector<std::thread*>::const_reverse_iterator it = Threads::vecBegin(); it != Threads::vecEnd(); it++) {
		(*it)->detach();
		(*it)->~thread();
	}

	Sleep(100);
	ParticleData::partFile << "\".split(\"|\")";
	ParticleData::partFile.close();
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

	printf("GL_MAX_COMPUTE_WORK_GROUP_COUNT:\n\tx:%I64u\n\ty:%I64u\n\tz:%I64u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &val_array[0]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &val_array[1]);
	glGetInteger64i_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &val_array[2]);

	printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE:\n\tx:%I64u\n\ty:%I64u\n\tz:%I64u\n",
		val_array[0], val_array[1], val_array[2]);

	glGetInteger64v(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
	printf("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &value);
	printf("GL_MAX_SHADER_STORAGE_BLOCK_SIZE :\n\t%I64u\n", value);

	glGetInteger64v(GL_SHADER_STORAGE_BUFFER_SIZE, &value);
	printf("GL_SHADER_STORAGE_BUFFER_SIZE :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_TEXTURE_SIZE, &value);
	printf("GL_MAX_TEXTURE_SIZE :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
	printf("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS  :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &value);
	printf("GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS   :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
	printf("GL_MAX_UNIFORM_BUFFER_BINDINGS    :\n\t%I64u\n", value);

	glGetInteger64v(GL_UNIFORM_BUFFER_SIZE, &value);
	printf("GL_UNIFORM_BUFFER_SIZE   :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, &value);
	printf("GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS     :\n\t%I64u\n", value);

	glGetInteger64v(GL_ATOMIC_COUNTER_BUFFER_SIZE, &value);
	printf("GL_ATOMIC_COUNTER_BUFFER_SIZE   :\n\t%I64u\n", value);

	glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
	printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE   :\n\t%I64u\n", value);

	checkOpenGLErrors();
}


