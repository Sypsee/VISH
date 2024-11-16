#include "Cube.h"

Cube::Cube()
{
	VertexArray::AttribInfo positionAttrib{ sizeof(float) * 3, GL_FLOAT, 0, sizeof(float) * 6, 0 };
	VertexArray::AttribInfo normalAttrib{ sizeof(float) * 3, GL_FLOAT, sizeof(float) * 3, sizeof(float) * 6, 1};
	VertexArray::AttribInfo attribs[] = { positionAttrib, normalAttrib };
	VertexArray::BufferInfo vertexBufferInfo
	{
		m_VertexBuffer, 0, sizeof(float) * 6, 0, std::span<VertexArray::AttribInfo>(attribs, 1)
	};
	VertexArray::BufferInfo vertexBuffers[] = { vertexBufferInfo };
	VertexArray::CreateInfo vaCreateInfo
	{
		std::span<VertexArray::BufferInfo>(vertexBuffers, 1), false, {{}}
	};
	VertexArray va{ vaCreateInfo };
	Mesh::CreateInfo meshCreateInfo
	{
		std::move(va)
	};
	m_Mesh = Mesh(meshCreateInfo);

	m_Shader.AttachShader({ "res/shaders/cube.vert", GL_VERTEX_SHADER });
	m_Shader.AttachShader({ "res/shaders/cube.frag", GL_FRAGMENT_SHADER });
}

void Cube::Draw(DrawInfo drawInfo)
{
	m_Shader.Bind();
	m_Shader.setMat4("proj", drawInfo.proj);
	m_Shader.setMat4("view", drawInfo.view);

	glm::mat4 model{ 1.0f };
	model = glm::translate(model, transform.pos);
	model = glm::rotate(model, transform.rot.x, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, transform.rot.y, glm::vec3(-1.f, 0.f, 0.f));
	model = glm::rotate(model, transform.rot.z, glm::vec3(0.f, 0.f, -1.f));
	model = glm::scale(model, transform.scale);

	m_Shader.setMat4("model", model);
	m_Shader.setVec3("viewPos", drawInfo.viewPos);

	m_Shader.setVec3("lightPos", drawInfo.light.pos);
	m_Shader.setVec3("lightColor", drawInfo.light.color);

	m_Mesh.Draw(36);
}