#pragma once

#include "../Game/Objects/Cube.h"
#include "../Game/Camera.h"
#include "Window.h"


class Application
{
public:
	static constexpr int START_WIDTH = 800;
	static constexpr int START_HEIGHT = 600;

	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void run();

private:
	void handleInputs();
	static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

	Window m_Window{ "VISH", START_WIDTH, START_HEIGHT };
	inline static Camera cam{glm::vec3(0, 0, 0)};
	Cube cube;
};