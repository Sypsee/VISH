#pragma once

#include "Mesh.h"
#include "../../OpenGL/Shader.h"
#include "ObjectInfo.h"

class Cube
{
public:
	struct DrawInfo
	{
		glm::mat4 proj{ 1.0 };
		glm::mat4 view{ 1.0 };
		glm::vec3 viewPos{ 0.0 };

		Light light;
	};

	Cube();
	void Draw(DrawInfo drawInfo);

	Transform transform;

private:
	// this is a mess....
	const float m_CubeVertices[36 * 6] = {
		1, -1, 1, 0, 0, 1,
		-1, 1, 1, 0, 0, 1,
		-1, -1, 1, 0, 0, 1,
		1, -1, 1, 0, 0, 1,
		1, 1, 1, 0, 0, 1,
		-1, 1, 1, 0, 0, 1,
		-1, 1, -1, 0, 0, -1,
		1, 1, -1, 0, 0, -1,
		1, -1, -1, 0, 0, -1,
		-1, -1, -1, 0, 0, -1,
		-1, 1, -1, 0, 0, -1,
		1, -1, -1, 0, 0, -1,
		-1, 1, 1, 0, 1, 0,
		1, 1, 1, 0, 1, 0,
		1, 1, -1, 0, 1, 0,
		-1, 1, -1, 0, 1, 0,
		-1, 1, 1, 0, 1, 0,
		1, 1, -1, 0, 1, 0,
		1, -1, -1, 0, -1, 0,
		-1, -1, 1, 0, -1, 0,
		-1, -1, -1, 0, -1, 0,
		1, -1, -1, 0, -1, 0,
		1, -1, 1, 0, -1, 0,
		-1, -1, 1, 0, -1, 0,
		1, 1, -1, 1, 0, 0,
		1, -1, 1, 1, 0, 0,
		1, -1, -1, 1, 0, 0,
		1, 1, -1, 1, 0, 0,
		1, 1, 1, 1, 0, 0,
		1, -1, 1, 1, 0, 0,
		-1, -1, 1, -1, 0, 0,
		-1, 1, 1, -1, 0, 0,
		-1, 1, -1, -1, 0, 0,
		-1, -1, -1, -1, 0, 0,
		-1, -1, 1, -1, 0, 0,
		-1, 1, -1, -1, 0, 0,
	};

	Buffer m_VertexBuffer{{GL_ARRAY_BUFFER, m_CubeVertices, sizeof(m_CubeVertices), GL_STATIC_DRAW}};

	Shader m_Shader;
	Mesh m_Mesh;
};