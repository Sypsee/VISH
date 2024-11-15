#include "Cube.h"

Cube::Cube()
{
	VertexArray::AttribInfo positionAttrib{ sizeof(float) * 3, GL_FLOAT, 0, sizeof(float) * 3 };
	VertexArray::AttribInfo attribs[] = { positionAttrib };
	VertexArray::BufferInfo vertexBufferInfo
	{
		m_VertexBuffer, 0, sizeof(float) * 3, 0, std::span<VertexArray::AttribInfo>(attribs, 1)
	};
	VertexArray::BufferInfo vertexBuffers[] = { vertexBufferInfo };
	VertexArray::CreateInfo vaCreateInfo
	{
		std::span<VertexArray::BufferInfo>(vertexBuffers, 1), true, m_IndexBuffer
	};
	VertexArray va{ vaCreateInfo };
	Mesh::CreateInfo meshCreateInfo
	{
		std::move(va), GL_UNSIGNED_INT
	};
	m_Mesh = Mesh(meshCreateInfo);

	m_Shader.AttachShader({ "res/shaders/cube.vert", GL_VERTEX_SHADER });
	m_Shader.AttachShader({ "res/shaders/cube.frag", GL_FRAGMENT_SHADER });
}

void Cube::Draw(Transform transform)
{
	m_Shader.Bind();
	m_Shader.setMat4("proj", transform.proj);
	m_Shader.setMat4("view", transform.view);
	m_Shader.setMat4("model", transform.model);

	m_Mesh.Draw(36);
}