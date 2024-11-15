#include <glad/glad.h>
#include "Application.h"
#include <iostream>

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
	glDepthFunc(GL_LESS);
    //glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
	//glEnable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, nullptr);
}

Application::~Application()
{
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

		glClearColor(0.05, 0.1, 0.15, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.Draw({cam.getProjMatrix(), cam.getViewMatrix()});

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