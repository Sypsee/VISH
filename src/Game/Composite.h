#pragma once

#include "../OpenGL/Shader.h"
#include "../OpenGL/VertexArray.h"
#include "Objects/Mesh.h"

class Composite
{
public:
	Composite(Shader&& shader);

	struct DrawInfo
	{

	};

	void Draw(DrawInfo drawInfo);

private:
	const float m_Vertices[5 * 6]{
		-1.0F,-1.0F, 0.0F, 0.0F, 0.0F,
		 1.0F,-1.0F, 0.0F, 1.0F, 0.0F,
		 1.0F, 1.0F, 0.0F, 1.0F, 1.0F,
		 1.0F, 1.0F, 0.0F, 1.0F, 1.0F,
		-1.0F, 1.0F, 0.0F, 0.0F, 1.0F,
		-1.0F,-1.0F, 0.0F, 0.0F, 0.0F
	};

	Buffer m_VertexBuffer{{GL_ARRAY_BUFFER, m_Vertices, sizeof(m_Vertices), GL_STATIC_DRAW}};

	Shader m_Shader;
	Mesh m_Mesh;
};