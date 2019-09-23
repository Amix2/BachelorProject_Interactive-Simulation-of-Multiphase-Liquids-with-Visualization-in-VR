
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <stdlib.h> 
#include "scene/Window.h"
#include "scene/ViewPort.h"
#include "scene/Scene.h"
#include "scene/Camera.h"
#include "scene/TestMaterialObject.h"
#include "scene/OcularCameraController.h"
#include "scene/TestBilboardObject.h"
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
#include <VR/VRCore.h>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/InputConfig.h>
#include <memory>

void printWorkGroupsCapabilities();

// init app elements
void initTools();

// atExit function
void cleanUp();


// settings
std::string NAME = "Random window";
constexpr unsigned int SCR_WIDTH = 1200;
constexpr unsigned int SCR_HEIGHT = 600;

const static std::unique_ptr<VR::VRCore> VrCore = std::make_unique<VR::VRCore>();
const static std::unique_ptr<VR::VRGeometry> VrGeometry = std::make_unique<VR::VRGeometry>();
const static std::unique_ptr<VR::VRInput> VrInput = std::make_unique<VR::VRInput>();

int main(int argc, char ** argv) {
	if (!VrCore->InitializeCore()) {
		std::cerr << "Couldn't init VR Core!" << std::endl;
	}
	else {
		VrInput->InitializeVRInput(std::string(VR::ACTIONS_PATH));
		VrGeometry->SetIVRSystem(VrCore->GetVrSystem());
	}

	if (LOG_TO_FILE) {
		ParticleData::partFile.open("./Simple Visualizer/part.log");	  
		ParticleData::partFile << "const partString = \"";
	}
	loguru::g_preamble_date = false;
	loguru::init(argc, argv);
	//loguru::add_file("log.log", loguru::Truncate, loguru::Verbosity_MAX);

	atexit(cleanUp);


/////////////////////////////////////////////////////////////////////////////////////

	Window window{ SCR_WIDTH, SCR_HEIGHT, NAME };
	if (window.init() == false) {
		std::cout << "Failed to init window";
		exit(1);
	}

	Scene::Scene scene{};

	ViewPort leftEyeViewPort{ window, 0.0f, 0.0f, 0.5f, 1.0f };;
	ViewPort rightEyeViewPort{ window, 0.5f, 0.0f, 0.5f, 1.0f };
	OcularCameraController cameraController{ window, leftEyeViewPort, rightEyeViewPort, 3.0f, glm::vec3{ -5.0f, 0.0f, 0.0f } };

	ShaderProgram programCubes{ "./Source/scene/testObject.v", "./Source/scene/testObject.f" };
	TestMaterialObject cubes{ programCubes };

	ShaderProgram programBilboard{ "./Source/scene/bilboard.vs", "./Source/scene/bilboard.fs" };
	TestBilboardObject bilboard{ programBilboard };

	//ShaderProgram 

/////////////////////////////////////////////////////////////////////////////////////

	initTools();

	Simulation::startSimulation(window.glfwWindow);
	//ParticleData::initArraysOnGPU();
	//printWorkGroupsCapabilities();

	scene.addCamera(&cameraController.getLeftCamera());
	scene.addCamera(&cameraController.getRightCamera());
    scene.addMaterialObject(&cubes);
	scene.addMaterialObject(&bilboard);

	do 
	{
		VrGeometry->SetupCameras();
		VrGeometry->UpdateHMDMatrixPose();
		VrInput->DetectPressedButtons();
		VrInput->HandleInput();
		cameraController.setHead(VrGeometry->TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
		std::cout << VrGeometry->TrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking.m[0][0] << std::endl;
		window.processInput();
		scene.renderScene();
	} while (!window.refresh());

	glfwTerminate();
	return 0;
}

void initTools()
{
	ParticleObjectCreator::init();
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

	glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
	printf("GL_MAX_COMPUTE_SHARED_MEMORY_SIZE   :\n\t%I64u\n", value);
	value = -1;


	checkOpenGLErrors();
}
