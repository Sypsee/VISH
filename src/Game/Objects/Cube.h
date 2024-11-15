#pragma once

#include "Mesh.h"
#include "../../OpenGL/Shader.h"

class Cube
{
public:
	struct Transform
	{
		glm::mat4 proj{ 1.0 };
		glm::mat4 view{ 1.0 };
		glm::mat4 model{1.0};
	};

	Cube();

	void Draw(Transform transform);

private:
	const float m_CubeVertices[24*3]{
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f, 
	};

	const unsigned int m_CubeIndices[6*6]{
		// front and back
		0, 3, 2,
		2, 1, 0,
		4, 5, 6,
		6, 7 ,4,
		// left and right
		11, 8, 9,
		9, 10, 11,
		12, 13, 14,
		14, 15, 12,
		// bottom and top
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	Buffer m_VertexBuffer{{GL_ARRAY_BUFFER, m_CubeVertices, sizeof(m_CubeVertices), GL_STATIC_DRAW}};
	Buffer m_IndexBuffer{{GL_ELEMENT_ARRAY_BUFFER, m_CubeIndices, sizeof(m_CubeIndices), GL_STATIC_DRAW}};

	Shader m_Shader;
	Mesh m_Mesh;
};