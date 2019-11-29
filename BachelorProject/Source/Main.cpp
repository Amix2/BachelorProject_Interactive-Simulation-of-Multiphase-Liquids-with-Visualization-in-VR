
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
#include <materialObjects/PyramidPointerMaterialObject.h>
#include <materialObjects/MoveIndicatorObject.h>
#include <VR/VRCore.h>
#include <VR/VRGeometry.h>
#include <VR/VRInput.h>
#include <VR/InputConfig.h>
#include <VR/VRGLInterop.h>
#include <memory>
#include <glassController/GlassController.h>
#include <Configuration.h>
#include <window/WindowTitle.h>
#include <emiters/EmiterManager.h>
#include <emiters/Emiter.h>
#include <window/FrameBuffer.h>
#include <utilities/GraphicShaderStorage.h>
#include <scene/camera/EmittingCameraController.h>
#include <digitalHand/DigitalHand.h>

void printWorkGroupsCapabilities();

// init app elements
void initTools(ShaderProgram programPyramidPointer);

// atExit function
void cleanUp();

void assignHardwareParameters();

//creates objects presented on scene
void setupScene(Scene::Scene& scene, InputDispatcher& inputDispatcher, const VR::VRGLInterop& vrglinterop, GlassController&);

// settings
std::string NAME = "Random window";
//H1680
//W1512
constexpr unsigned int VR_SCR_WIDTH = 1512 * 2;
constexpr unsigned int VR_SCR_HEIGHT = 1680;
constexpr unsigned int WINDOW_SCR_WIDTH = 1600;
constexpr unsigned int WINDOW_SCR_HEIGHT = 900;

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
	Window window{ WINDOW_SCR_WIDTH, WINDOW_SCR_HEIGHT, NAME, inputDispatcher };
	if (window.init() == false) {
		LOG_F(ERROR, "Failed to init window");
		exit(1);
	}

	WindowTitle::setWindow(window.glfwWindow);

	Scene::Scene scene{ Configuration::BACKGROUND, 2 };
	VR::VRGLInterop vrglinterop{ VR_SCR_WIDTH, VR_SCR_HEIGHT };
	vrglinterop.init();


/////////////////////////////////////////////////////////////////////////////////////
	static ShaderProgram programPyramidPointer{ "./Source/shaders/pyramidPointer/PyramidPointer.vert", "./Source/shaders/pyramidPointer/PyramidPointer.frag" };

	assignHardwareParameters();
	initTools(programPyramidPointer);

	Simulation::startSimulation(window.glfwWindow);

	CameraController* cameraController = nullptr;
	FrameBuffer* vrFrameBuffer = nullptr;

	if ((HmdConnected = vrglinterop.hasVR())) {
		vrFrameBuffer = new FrameBuffer{ VR_SCR_WIDTH, VR_SCR_HEIGHT };
		vrFrameBuffer->init();
		ViewPort leftEyeViewPort{ *vrFrameBuffer,  0.0f, 0.0f, 0.5f, 1.0f };
		ViewPort rightEyeViewPort{ *vrFrameBuffer, 0.5f, 0.0f, 0.5f, 1.0f };
		cameraController = new VRCameraController{ leftEyeViewPort, rightEyeViewPort, 0.64f };
	}
	else {
		LOG_F(INFO, "Couldn't init VR Core - switching to regular display mode");

		ViewPort viewPort{ window, 0.0f, 0.0f, 1.0f, 1.0f };
		cameraController = new EmittingCameraController{ inputDispatcher, viewPort, glm::vec3{ 100,50, 100 } };
	}

	scene.addCameras(cameraController);
	
	ShaderProgram programGlass{ "./Source/shaders/glass/glass.vert", "./Source/shaders/glass/glass.frag" }; 
	GraphicShaderStorage::addShader(ShaderNames.GlassObject, programGlass);
	ShaderProgram programSelectedGlass{ "./Source/shaders/glass/selected/glass.vert", "./Source/shaders/glass/selected/glass.frag" };
	GraphicShaderStorage::addShader(ShaderNames.SelectedGlassObject, programSelectedGlass);
	GlassController glassController{ inputDispatcher, *cameraController->provideCameras().at(0), programGlass, programSelectedGlass };

	setupScene(scene, inputDispatcher, vrglinterop, glassController);
	//EmiterManager::setEmiter(cameraController, 25, 1000.0f);
	//EmiterManager::setInputDispatcher(&inputDispatcher);
	//EmiterManager::createEmiter(10, 100.f, 1, { 10, 0, 10 }, true);
	//EmiterManager::createEmiter(10, 100.f, 1, { 10,0,10 }, true);
	HandDataProvider handDataProvider;
	DigitalHand leftHand(&handDataProvider, LEFT_HAND, programPyramidPointer, &vrglinterop);
	DigitalHand rightHand(&handDataProvider, RIGHT_HAND, programPyramidPointer, &vrglinterop);
	scene.addMaterialObject(&leftHand, 0);
	scene.addMaterialObject(&rightHand, 0);

	//EmiterManager::createEmiter(10, 1000.f, 1, { 0, 0, 0 }, true);
	//EmiterManager::createEmiter(10, 1000.f, 1, { 0, 50, 0 }, true);
	EmiterManager::createEmiter(10, 1000.f, 1, { 0, 100, 0 }, true);

	do 
	{
		glassController.assignUntrackedObjects(scene);
		if (HmdConnected) {
			vrglinterop.handleInput(static_cast<VRCameraController*>(cameraController));
			leftHand.update();
			rightHand.update();
			vrFrameBuffer->drawTo();
		}

		glassController.assignUntrackedObjects(scene);
		scene.renderScene();

		if (HmdConnected) {
			vrglinterop.sumbitFrame(*vrFrameBuffer);
			window.getFrameFrom(*vrFrameBuffer);
		}
		window.refresh();
		//SelectableObjectManager::m_selectableObjects;
		//for (SelectableObject* object : SelectableObjectManager::m_selectableObjects) object->getVRActionController()->triggerButton();
	} while (!window.shouldClose());

	glfwTerminate();
	return 0;
}


void setupScene(Scene::Scene& scene, InputDispatcher& inputDispatcher, const VR::VRGLInterop& vrglinterop, GlassController& glassController) {
	//static ShaderProgram programCubes{ "./Source/shaders/testObject/testObject.vert", "./Source/shaders/testObject/testObject.frag" };
	//static TestMaterialObject cubes{ programCubes, scene.getBackgroundColor() };

	//static ShaderProgram programBilboard{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.geom", "./Source/shaders/particles/particles.frag" };
	//static TestBilboardObject bilboard{ programBilboard };

	static ShaderProgram programFluid{ "./Source/shaders/particles/particles.vert", "./Source/shaders/particles/particles.frag" };
	static FluidObject fluid{ inputDispatcher, programFluid };

	static ShaderProgram programAxes{ "./Source/shaders/axes/axes.vert", "./Source/shaders/axes/axes.frag" };
	static AxesObject axes{ programAxes };

	static ShaderProgram programVectorNormals{ "./Source/shaders/normalVectors/normalVectors.vert", "./Source/shaders/normalVectors/normalVectors.geom", "./Source/shaders/normalVectors/normalVectors.frag" };
	static NormalVectorsObject vectorNormals{ inputDispatcher, programVectorNormals };

	static ShaderProgram moveIndicatorProgram{ "./Source/shaders/moveIndicator/moveIndicator.vert", "./Source/shaders/moveIndicator/moveIndicator.frag" };
	static MoveIndicatorObject moveIndicatorObject{ inputDispatcher, moveIndicatorProgram, &glassController };

	//scene.addMaterialObject(&cubes);
	//scene.addMaterialObject(&bilboard);
	scene.addMaterialObject(&fluid, 1);
	scene.addMaterialObject(&axes, 1);
	scene.addMaterialObject(&vectorNormals, 1);
	//scene.addMaterialObject(&moveIndicatorObject, 1);
}

void initTools(ShaderProgram programPyramidPointer)
{
	WindowTitle::init();

	ParticleObjectManager::init();
	FluidType::init();
	ShaderCodeEditor::init();
	EmiterManager::init(programPyramidPointer);

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

	while(Configuration.MAX_PARTICLES > maxNumOfParticles) Configuration.MAX_PARTICLES *= 0.5;
	while(Configuration.MAX_GLASS_PARTICLES > maxNumOfGlassParticles) Configuration.MAX_GLASS_PARTICLES *= 0.5;

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
