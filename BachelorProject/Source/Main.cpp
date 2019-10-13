
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

static bool HmdConnected;

int main(int argc, char ** argv) {
	if (!VrCore->InitializeCore()) {
		std::cerr << "Couldn't init VR Core!" << std::endl;
		HmdConnected = false;
	}
	else {
		VrInput->InitializeVRInput(std::string(VR::ACTIONS_PATH));
		VrGeometry->SetIVRSystem(VrCore->GetVrSystem());
		HmdConnected = true;
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

	GLuint m_nResolveFramebufferIdLeft;
	GLuint m_nRenderFramebufferIdLeft;
	GLuint m_nDepthBufferIdLeft;
	GLuint m_nRenderTextureIdLeft;
	GLuint m_nResolveTextureIdLeft;

	GLuint m_nResolveFramebufferIdRight;
	GLuint m_nRenderFramebufferIdRight;
	GLuint m_nDepthBufferIdRight;
	GLuint m_nRenderTextureIdRight;
	GLuint m_nResolveTextureIdRight;

	////////////////////

	glGenFramebuffers(1, &m_nResolveFramebufferIdLeft);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

	glGenFramebuffers(1, &m_nRenderFramebufferIdLeft);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nRenderFramebufferIdLeft);

	glGenRenderbuffers(1, &m_nDepthBufferIdLeft);
	glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBufferIdLeft);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBufferIdLeft);

	glGenTextures(1, &m_nRenderTextureIdLeft);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdLeft);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdLeft, 0);

	glGenFramebuffers(1, &m_nResolveFramebufferIdLeft);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

	glGenTextures(1, &m_nResolveTextureIdLeft);
	glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdLeft);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nResolveTextureIdLeft, 0);

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///////////////////////

	////////////////////

	glGenFramebuffers(1, &m_nResolveFramebufferIdRight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdRight);

	glGenFramebuffers(1, &m_nRenderFramebufferIdRight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nRenderFramebufferIdRight);

	glGenRenderbuffers(1, &m_nDepthBufferIdRight);
	glBindRenderbuffer(GL_RENDERBUFFER, m_nDepthBufferIdRight);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nDepthBufferIdRight);

	glGenTextures(1, &m_nRenderTextureIdRight);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdRight);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_nRenderTextureIdRight, 0);

	glGenFramebuffers(1, &m_nResolveFramebufferIdRight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nResolveFramebufferIdRight);

	glGenTextures(1, &m_nResolveTextureIdRight);
	glBindTexture(GL_TEXTURE_2D, m_nResolveTextureIdRight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nResolveTextureIdRight, 0);

	// check FBO status
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///////////////////////

	do 
	{
		if (HmdConnected) {
			///////////////////////////////////////////////////////////
			glClearColor(0.3f, 0.5f, 0.4f, 1.0f);
			glEnable(GL_MULTISAMPLE);

			// Left Eye
			glBindFramebuffer(GL_FRAMEBUFFER, m_nRenderFramebufferIdLeft);
			// ????? glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
			// RenderScene(vr::Eye_Left);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			window.processInput();
			scene.renderScene();

			//
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_nRenderFramebufferIdLeft);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_nResolveFramebufferIdLeft);

			glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
				GL_COLOR_BUFFER_BIT,
				GL_LINEAR);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			//////////////////////////////////////////////////////////////

			VrGeometry->SetupCameras();
			VrGeometry->UpdateHMDMatrixPose();
			VrInput->DetectPressedButtons();
			VrInput->HandleInput();
			cameraController.setHead(VrGeometry->TrackedDevicePose.mDeviceToAbsoluteTracking);
			auto [LeftTexture, RightTexture] = VrGeometry->ObtainTextures(m_nResolveTextureIdLeft, m_nResolveTextureIdRight);
			vr::TrackedDevicePose_t xxx[vr::k_unMaxTrackedDeviceCount];
			std::cout << vr::VRCompositor()->WaitGetPoses(xxx, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
			VrCore->SubmitTexturesToHMD(LeftTexture, RightTexture);
		}
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
