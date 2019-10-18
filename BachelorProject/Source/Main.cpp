
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <stdlib.h> 
#include "./window/Window.h"
#include "./window/ViewPort.h"
#include "scene/Scene.h"
#include "scene/camera/Camera.h"
#include "materialObjects/TestMaterialObject.h"
#include <materialObjects/GlassObject.h>
#include "scene/camera/VRCameraController.h"
#include "materialObjects/TestBilboardObject.h"
#include "materialObjects/FluidObject.h"
#include "scene/camera/SimpleCameraController.h"
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
#include <thread>
#include <materialObjects/AxesObject.h>
#include <materialObjects/NormalVectorsObject.h>
#include <glassController/GlassController.h>
#include <Configuration.h>

void printWorkGroupsCapabilities();

// init app elements
void initTools();

// atExit function
void cleanUp();

void assignHardwareParameters();


// settings
std::string NAME = "Random window";
constexpr unsigned int SCR_WIDTH = 1600;
constexpr unsigned int SCR_HEIGHT = 900;

int main(int argc, char ** argv) {

	if (LOG_TO_FILE) {
		ParticleData::partFile.open("./Simple Visualizer/part.log");	  
		ParticleData::partFile << "const partString = \"";
	}
	loguru::g_preamble_date = false;
	loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;	// show only ERRORS
	loguru::init(argc, argv);
	//loguru::add_file("log.log", loguru::Truncate, loguru::Verbosity_MAX);

	atexit(cleanUp);



/////////////////////////////////////////////////////////////////////////////////////
	

	InputDispatcher inputDispatcher;
	Window window{ SCR_WIDTH, SCR_HEIGHT, NAME, inputDispatcher };
	if (window.init() == false) {
		std::cout << "Failed to init window";
		exit(1);
	}

	Scene::Scene scene{ Configuration::BACKGROUND };

	//if(headsetIsAvaiable){
	//ViewPort leftEyeViewPort{ window, 0.0f, 0.0f, 0.5f, 1.0f };;
	//ViewPort rightEyeViewPort{ window, 0.5f, 0.0f, 0.5f, 1.0f };
	//VRCameraController cameraController{ window, leftEyeViewPort, rightEyeViewPort, 3.0f, glm::vec3{ -5.0f, 0.0f, 0.0f } };
	//scene.addCamera(&cameraController.getLeftCamera());
	//scene.addCamera(&cameraController.getRightCamera());
	//}
	//else 
	//{
	ViewPort viewPort{ window, 0.0f, 0.0f, 1.0f, 1.0f };
	SimpleCameraController cameraController{ inputDispatcher, viewPort, glm::vec3{ 0,40, 0 } };
	scene.addCamera(&cameraController.getCamera());
	//}

	//ShaderProgram programCubes{ "./Source/shaders/testObject/testObject.vert", "./Source/shaders/testObject/testObject.frag" };
	//TestMaterialObject cubes{ programCubes, backgroundColor };

	//ShaderProgram programBilboard{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.geom", "./Source/shaders/particles/particles.frag" };
	//TestBilboardObject bilboard{ programBilboard };

	ShaderProgram programFluid{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.geom", "./Source/shaders/particles/particles.frag" };
	FluidObject fluid{ inputDispatcher, programFluid };

	ShaderProgram programAxes{ "./Source/shaders/axes/axes.vert", "./Source/shaders/axes/axes.frag" };
	AxesObject axes{ programAxes };

	ShaderProgram programVectorNormals{ "./Source/shaders/normalVectors/normalVectors.vert", "./Source/shaders/normalVectors/normalVectors.geom", "./Source/shaders/normalVectors/normalVectors.frag" };
	NormalVectorsObject vectorNormals{ inputDispatcher, programVectorNormals };

/////////////////////////////////////////////////////////////////////////////////////

	assignHardwareParameters();

	initTools();

	Simulation::startSimulation(window.glfwWindow);
	
	//printWorkGroupsCapabilities();

	ShaderProgram programGlass{ "./Source/shaders/glass/glass.vert", "./Source/shaders/glass/glass.frag" }; 
	ShaderProgram programSelectedGlass{ "./Source/shaders/glass/selected/glass.vert", "./Source/shaders/glass/selected/glass.frag" };
	GlassController glassController{ inputDispatcher, cameraController.getCamera(), programGlass, programSelectedGlass };


	//scene.addMaterialObject(&cubes);
	//scene.addMaterialObject(&bilboard);
	scene.addMaterialObject(&axes);
	scene.addMaterialObject(&vectorNormals);
	scene.addMaterialObject(&fluid);

	do 
	{
		glassController.assignUntrackedObjects(scene);
		scene.renderScene();
	} while (!window.refresh());

	glfwTerminate();
	return 0;
}

void initTools()
{
	ParticleObjectManager::init();
	FluidType::init();
	ShaderCodeEditor::init();

	ParticleData::initArraysOnGPU();	// HAS to be fired at the end (FluidType must be initialized before)
}

void cleanUp()
{
	LOG_F(INFO, "Clean Up");
	for (std::vector<std::thread*>::const_reverse_iterator it = Threads::vecBegin(); it != Threads::vecEnd(); it++) {
		(*it)->detach();
		(*it)->~thread();
	}
	if (LOG_TO_FILE) {
		ParticleData::partFile << "\".split(\"|\")";
		ParticleData::partFile.close();
	}
}

void assignHardwareParameters()
{
	GLint64  SSBOsize;

	glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &SSBOsize);

	const int maxNumOfParticles = SSBOsize / (27 * sizeof(float));
	const int maxNumOfGlassParticles = SSBOsize / sizeof(GlassParticle);

	Configuration.MAX_PARTICLES = std::min(maxNumOfParticles, Configuration.MAX_PARTICLES);
	Configuration.MAX_GLASS_PARTICLES = std::min(maxNumOfGlassParticles, Configuration.MAX_GLASS_PARTICLES);
	const int baseX = Configuration.SCENE_SIZE_X;
	const int baseY = Configuration.SCENE_SIZE_Y;
	const int baseZ = Configuration.SCENE_SIZE_Z;
	while ((long)Configuration.SCENE_SIZE_X * (long)Configuration.SCENE_SIZE_Y * (long long)Configuration.SCENE_SIZE_Z * (long)sizeof(GLuint) < SSBOsize) {
		Configuration.SCENE_SIZE_X += baseX;
		Configuration.SCENE_SIZE_Y += baseY;
		Configuration.SCENE_SIZE_Z += baseZ;
	}
	Configuration.SCENE_SIZE_X -= baseX;
	Configuration.SCENE_SIZE_Y -= baseY;
	Configuration.SCENE_SIZE_Z -= baseZ;
	LOG_F(WARNING, "Simulation scene size: (%d %d %d), Max particles: %d, Max glass particles: %d"
		, Configuration.SCENE_SIZE_X, Configuration.SCENE_SIZE_Y, Configuration.SCENE_SIZE_Z, Configuration.MAX_PARTICLES, Configuration.MAX_GLASS_PARTICLES);
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

	glGetInteger64v(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS, &value);
	printf("GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS   :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &value);
	printf("GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS   :\n\t%I64u\n", value);
	value = -1;

	glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
	printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE   :\n\t%I64u\n", value);
	value = -1;


	checkOpenGLErrors();
}
