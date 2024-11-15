#include <glad/glad.h>
#include "Window.h"

#include <iostream>

Window::Window(const char* title, const int width, const int height)
	:m_WindowRes{width, height}
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cerr << "Error while creating glfwWindow!" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
{
    auto myWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    myWindow->m_FramebufferResized = true;
    myWindow->m_WindowRes.x = width;
    myWindow->m_WindowRes.y = height;
}