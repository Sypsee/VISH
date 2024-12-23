#pragma once

#include <glad/glad.h>
#include "../Game/Camera.h"
#include "../Opengl/Framebuffer.h"
#include "../Game/Objects/Model.h"
#include "../Game/Composite.h"
#include "Window.h"

#include <array>

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
	std::vector<Light> lights;
	
	Model model{ std::filesystem::path("res/models/berserk-gutsver2/scene.gltf") };

	Framebuffer *m_FB;
	Composite* comp;
};