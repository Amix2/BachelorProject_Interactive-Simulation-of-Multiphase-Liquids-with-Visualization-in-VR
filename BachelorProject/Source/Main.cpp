
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
#include <scene/camera/SimpleCameraController.h>
#include <scene/camera/VRCameraController.h>
#include <scene/camera/CameraController.h>
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
#include <VR/VRCore.h>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/InputConfig.h>
#include <VR/VRGLInterop.h>
#include <memory>
#include <glassController/GlassController.h>
#include <Configuration.h>

void printWorkGroupsCapabilities();

// init app elements
void initTools();

// atExit function
void cleanUp();

void assignHardwareParameters();

//creates objects presented on scene
void setupScene(Scene::Scene& scene, InputDispatcher& inputDispatcher);

// settings
std::string NAME = "Random window";
constexpr unsigned int SCR_WIDTH = 1600;
constexpr unsigned int SCR_HEIGHT = 900;

static bool HmdConnected;

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
	VR::VRGLInterop vrglinterop;
	vrglinterop.activate();

	CameraController* cameraController;
	ViewPort viewPort{ window, 0.0f, 0.0f, 1.0f, 1.0f };

	if ((HmdConnected = vrglinterop.hasVR())) {
		ViewPort leftEyeViewPort{ window, 0.0f, 0.0f, 0.5f, 1.0f };;
		ViewPort rightEyeViewPort{ window, 0.5f, 0.0f, 0.5f, 1.0f };
		cameraController = new VRCameraController{ leftEyeViewPort, rightEyeViewPort, 0.64f };
	}
	else {
		std::cerr << "Couldn't init VR Core!" << std::endl;

		ViewPort viewPort{ window, 0.0f, 0.0f, 1.0f, 1.0f };
		cameraController = new SimpleCameraController{ inputDispatcher, viewPort, glm::vec3{ 0,40, 0 } };
	}

	scene.addCameras(cameraController);

/////////////////////////////////////////////////////////////////////////////////////

	assignHardwareParameters();

	initTools();
	Simulation::startSimulation(window.glfwWindow);
	
	//printWorkGroupsCapabilities();

	ShaderProgram programGlass{ "./Source/shaders/glass/glass.vert", "./Source/shaders/glass/glass.frag" }; 
	ShaderProgram programSelectedGlass{ "./Source/shaders/glass/selected/glass.vert", "./Source/shaders/glass/selected/glass.frag" };
	GlassController glassController{ inputDispatcher, *cameraController->provideCameras().at(0), programGlass, programSelectedGlass };

	//ParticleData::initArraysOnGPU();
	printWorkGroupsCapabilities();


	setupScene(scene, inputDispatcher);

	do 
	{
		glassController.assignUntrackedObjects(scene);
		scene.renderScene();
		if (HmdConnected) {
			vrglinterop.sumbitFrame();
			vrglinterop.handleInput(static_cast<VRCameraController*>(cameraController));
		}
	} while (!window.refresh());

	glfwTerminate();
	return 0;
}










void setupScene(Scene::Scene& scene, InputDispatcher& inputDispatcher) {
	//static ShaderProgram programCubes{ "./Source/shaders/testObject/testObject.vert", "./Source/shaders/testObject/testObject.frag" };
	//static TestMaterialObject cubes{ programCubes, scene.getBackgroundColor() };

	//static ShaderProgram programBilboard{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.geom", "./Source/shaders/particles/particles.frag" };
	//static TestBilboardObject bilboard{ programBilboard };

	static ShaderProgram programFluid{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.geom", "./Source/shaders/particles/particles.frag" };
	static FluidObject fluid{ inputDispatcher, programFluid };

	static ShaderProgram programAxes{ "./Source/shaders/axes/axes.vert", "./Source/shaders/axes/axes.frag" };
	static AxesObject axes{ programAxes };

	static ShaderProgram programVectorNormals{ "./Source/shaders/normalVectors/normalVectors.vert", "./Source/shaders/normalVectors/normalVectors.geom", "./Source/shaders/normalVectors/normalVectors.frag" };
	static NormalVectorsObject vectorNormals{ inputDispatcher, programVectorNormals };

	//scene.addMaterialObject(&cubes);
	//scene.addMaterialObject(&bilboard);
	scene.addMaterialObject(&fluid);
	scene.addMaterialObject(&axes);
	scene.addMaterialObject(&vectorNormals);
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
