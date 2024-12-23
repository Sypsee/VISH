#include "Composite.h"

#include <array>

Composite::Composite(Shader&& shader)
{
	m_Shader = std::move(shader);

	VertexArray::AttribInfo positionAttrib{ 3, GL_FLOAT, 0, 0 };
	VertexArray::AttribInfo uvAttrib{ 2, GL_FLOAT, sizeof(float) * 3, 1 };

	std::array<VertexArray::AttribInfo, 2> attribs = {
		positionAttrib,
		uvAttrib
	};
	VertexArray::BufferInfo vertexBufferInfo
	{
		std::move(m_VertexBuffer), 0, sizeof(float) * 5, 0, attribs
	};
	VertexArray::BufferInfo vertexBuffers[] = { vertexBufferInfo };
	VertexArray::CreateInfo vaCreateInfo
	{
		std::span<VertexArray::BufferInfo>(vertexBuffers, 1), false, {{GL_NONE}}
	};
	VertexArray va{ vaCreateInfo };
	Mesh::CreateInfo meshCreateInfo
	{
		std::move(va)
	};
	m_Mesh = Mesh(meshCreateInfo);

	m_Shader.Bind();
	m_Shader.setI("u_ScreenTex", 0);
	m_Shader.setI("u_DepthTex", 1);
}

void Composite::Draw(DrawInfo drawInfo)
{
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	m_Shader.Bind();
	m_Mesh.Draw(6);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
}