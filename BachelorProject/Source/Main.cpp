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
#include "scene/TestBilboardObject.h"

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

	ViewPort leftEyeViewPort{ window, 0.0f, 0.0f, 0.5f, 1.0f };;
	ViewPort rightEyeViewPort{ window, 0.5f, 0.0f, 0.5f, 1.0f };
	OcularCameraController cameraController{ window, leftEyeViewPort, rightEyeViewPort, 3.0f, glm::vec3{ -5.0f, 0.0f, 0.0f } };

	ShaderProgram programCubes{ "./Source/scene/testObject.v", "./Source/scene/testObject.f" };
	TestMaterialObject cubes{ programCubes };

	ShaderProgram programBilboard{ "./Source/scene/bilboard.vs", "./Source/scene/bilboard.fs" };
	TestBilboardObject bilboard{ programBilboard };

	scene.addCamera(&cameraController.getLeftCamera());
	scene.addCamera(&cameraController.getRightCamera());
    scene.addMaterialObject(&cubes);
	scene.addMaterialObject(&bilboard);

	do 
	{
		window.processInput();
		scene.renderScene();
	} while (!window.refresh());

	glfwTerminate();
	return 0;
}