#pragma once

#include "Mesh.h"
#include "../../OpenGL/Shader.h"
#include "../../OpenGL/Texture.h"
#include "ObjectInfo.h"

#include <vector>

class Plane
{
public:
	struct CreateInfo
	{
		const int resolution = 10;
		const int width = 10;
		bool customShader = false;
		Shader&& shader{};
		Texture&& texture{ {} };
	};

	struct DrawInfo
	{
		glm::mat4 proj{ 1.0 };
		glm::mat4 view{ 1.0 };
	};

	Plane(CreateInfo const& createInfo);
	void Draw(DrawInfo drawInfo);

	Transform transform;

private:
	std::vector<float> m_Vertices{ 0.f };
	std::vector<unsigned int> m_Indices{ 0 };

	Buffer m_VertexBuffer{{GL_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW}};
	Buffer m_IndexBuffer{{GL_ELEMENT_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW}};

	Texture m_Texture;
	Shader m_Shader;
	Mesh m_Mesh;
};