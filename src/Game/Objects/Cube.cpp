#include "Cube.h"
#include <array>

Cube::Cube(CreateInfo const& createInfo)
{
	VertexArray::AttribInfo positionAttrib{ 3, GL_FLOAT, 0, 0 };
	VertexArray::AttribInfo normalAttrib{ 3, GL_FLOAT, sizeof(float) * 3, 1 };
	VertexArray::AttribInfo uvAttrib{ 2, GL_FLOAT, sizeof(float) * 6, 2 };

	std::array<VertexArray::AttribInfo, 3> attribs = {
		positionAttrib,
		normalAttrib,
		uvAttrib
	};
	VertexArray::BufferInfo vertexBufferInfo
	{
		m_VertexBuffer, 0, sizeof(float) * 8, 0, attribs
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

	if (createInfo.customShader)
	{
		m_Shader = std::move(createInfo.shader);
	}
	else
	{
		m_Shader.AttachShader({ "res/shaders/lit.vert", GL_VERTEX_SHADER });
		m_Shader.AttachShader({ "res/shaders/lit.frag", GL_FRAGMENT_SHADER });
	}

	if (createInfo.texture.getHandle() != 69)
	{
		m_Texture = std::move(createInfo.texture);
		m_Shader.setI("u_HasTex", true);
	}
	else
	{
		m_Shader.setI("u_HasTex", false);
	}

	m_Shader.setI("u_Tex", 0);
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

	if (m_Texture.getHandle() != 69)
	{
		m_Texture.bind();
	}

	m_Mesh.Draw(36);
}