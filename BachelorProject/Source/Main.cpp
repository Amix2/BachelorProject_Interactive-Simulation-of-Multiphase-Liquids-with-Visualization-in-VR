#include <glad/glad.h>
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

constexpr unsigned int SCR_WIDTH = 1200;
constexpr unsigned int SCR_HEIGHT = 600;
std::string NAME = "Random window";


int main(int argc, char ** argv) {

	Window window{ SCR_WIDTH, SCR_HEIGHT, NAME };
	if (window.init() == false) {
		std::cout << "Failed to init window";
		exit(1);
	}

	Scene::Scene scene{};

	ViewPort leftEyeViewPort{ window, 0.0f, 0.0f, 0.5f, 1.0f };
	glm::vec3 leftEyePosition{ -5.0f, 0.0f, -2.0f };
	Scene::Camera leftEyeCamera{ leftEyeViewPort, leftEyePosition };
	std::cout << &leftEyeCamera << std::endl;

	ViewPort rightEyeViewPort{ window, 0.5f, 0.0f, 0.5f, 1.0f };
	glm::vec3 rightEyePosition{ -5.0f, 0.0f, 0.0f };
	Scene::Camera rightEyeCamera{ rightEyeViewPort, rightEyePosition };
	std::cout << &rightEyeCamera << std::endl;

	OcularCameraController cameraController{ window, leftEyeCamera, rightEyeCamera };

	ShaderProgram program{ "./Source/scene/testObject.v", "./Source/scene/testObject.f" };
	TestMaterialObject cubes{ program };

	scene.addCamera(&leftEyeCamera);
	scene.addCamera(&rightEyeCamera);
	scene.addMaterialObject(&cubes);

	do 
	{
		window.processInput();
		scene.renderScene();
	} while (!window.refresh());

	glfwTerminate();
	return 0;
}