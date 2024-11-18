#include <glad/glad.h>
#include "Application.h"
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void Application::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    cam.mouse_callback(xpos, ypos);
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

Application::Application()
{
	glViewport(0, 0, START_WIDTH, START_HEIGHT);
	glfwSetCursorPosCallback(m_Window.getGLFWwindow(), mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	glEnable(GL_BLEND);
	glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, nullptr);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_Window.getGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::run()
{
	float currentFrame = glfwGetTime();
	float lastFrame = currentFrame;
	float dt = 0;

	while (!m_Window.shouldClose())
	{
		glfwPollEvents();
		handleInputs();
		
		currentFrame = glfwGetTime();
		dt = std::max(currentFrame - lastFrame, 0.05f);
		lastFrame = currentFrame;

		if (m_Window.wasWindowResized())
		{
			m_Window.resetWindowResizeFlag();
			glViewport(0, 0, m_Window.getWindowRes().x, m_Window.getWindowRes().y);
			cam.setAspectRatio(m_Window.getWindowRes().x/m_Window.getWindowRes().y);
		}

		cam.update(m_Window.getGLFWwindow());

		glClearColor(0.6, 0.9, 1.0, 1.0);
		glClearDepth(0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		cube.Draw({cam.getProjMatrix(), cam.getViewMatrix(), cam.getPosition(), light});

		ImGui::Begin("Cube!");
		ImGui::DragFloat3("Position", glm::value_ptr(cube.transform.pos), 0.3f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(cube.transform.rot), 0.3f);
		ImGui::DragFloat3("Scale", glm::value_ptr(cube.transform.scale), 0.3f);
		ImGui::End();

		ImGui::Begin("Light!");
		ImGui::DragFloat3("Position", glm::value_ptr(light.pos));
		ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window.getGLFWwindow());
	}
}

void Application::handleInputs()
{
	if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_O) == GLFW_PRESS)	// Wire frame
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	else if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_P) == GLFW_PRESS) // Default
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	else if (glfwGetKey(m_Window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) // Quit
	{
		glfwSetWindowShouldClose(m_Window.getGLFWwindow(), true);
	}
}