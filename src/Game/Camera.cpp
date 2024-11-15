#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(glm::vec3 camPos, float FOV, float nearClip, float farClip)
	:FOV(FOV), nearClip(nearClip), farClip(farClip), cameraPos(camPos)
{
	proj = getinfrzproj();
    inverseProj = glm::inverse(proj);
}

void Camera::update(GLFWwindow *window)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	process_inputs(window);

	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(cameraDirection);
	cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::process_inputs(GLFWwindow* window)
{
	float cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = 15.f * deltaTime;
	else
		cameraSpeed = 8.f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(cameraRight) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(cameraRight) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;

	if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
	{
		mouse_hidden = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (glfwGetMouseButton(window, 1) == GLFW_RELEASE)
	{
		mouse_hidden = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Camera::mouse_callback(double xpos, double ypos)
{
	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 2.f * deltaTime;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	if (!mouse_hidden) return;

	yaw += xoffset;
	pitch += yoffset;

	pitch = glm::clamp(pitch, -89.9f, 89.f);
}

void Camera::setAspectRatio(float aspect_ratio)
{
	aspectRatio = aspect_ratio;
	proj = getinfrzproj();
    inverseProj = glm::inverse(proj);
}

void Camera::setPosition(glm::vec3 pos)
{
	cameraPos = pos;
}

glm::vec3 Camera::getPosition() const
{
	return cameraPos;
}

glm::mat4 Camera::getProjMatrix() const
{
	return proj;
}

glm::mat4 Camera::getInverseProjMatrix() const
{
	return inverseProj;
}

glm::mat4 Camera::getinfrzproj() const
{
    float f = 1.0f / tan(glm::radians(FOV) / 2.0f);
    return glm::mat4(
    f / aspectRatio,    0.0f,  0.0f,  0.0f,
                  0.0f,    f,  0.0f,  0.0f,
                  0.0f, 0.0f,  0.0f, -1.0f,
                  0.0f, 0.0f,nearClip,0.0f);
}

glm::mat4 Camera::getViewMatrix() const
{
	return view;
}
