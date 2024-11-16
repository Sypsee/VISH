#pragma once

#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 pos{ 0.0 };
	glm::vec3 rot{ 0.0 };
	glm::vec3 scale{ 1.0 };
};

struct Light
{
	glm::vec3 pos{ 1.2f, 1.0f, 2.0f };
	glm::vec3 color{ 1.0 };
};