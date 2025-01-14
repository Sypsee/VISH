#pragma once

#include <glad/glad.h>
#include "../Game/Camera.h"
#include "../Opengl/Framebuffer.h"
#include "../Game/Objects/Plane.h"
#include "../Game/Objects/Cube.h"
#include "../Game/Objects/Model.h"
#include "../Game/Composite.h"
#include "Window.h"

#include <array>

class Application
{
public:
	static constexpr int START_WIDTH = 1100;
	static constexpr int START_HEIGHT = 620;

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
	std::vector<Light> lights;
	
	//Plane plane{ {} };
	Model model{ "res/models/Cube/SM_Deccer_Cubes_Textured.gltf" };

	Framebuffer *m_FB;
	
	Composite* m_GridComp;
	Composite* m_PostComp;

	Cube* cubemap;

	bool isWireframe = false;
};