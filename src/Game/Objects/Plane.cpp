#include "Plane.h"
#include <GLFW/glfw3.h>

#include <array>
#include <string>
#include <iostream>

Plane::Plane(CreateInfo const& createInfo)
{
	float halfLength = createInfo.width * 0.5f;
	int sideVertCount = createInfo.width * createInfo.resolution;

	m_Vertices.resize((sideVertCount + 1) * (sideVertCount + 1) * 8);

	for (int i = 0, x = 0; x <= sideVertCount; ++x)
	{
		for (int z = 0; z <= sideVertCount; ++z, i += 8)
		{
			m_Vertices[i] = (static_cast<float>(x) / sideVertCount * createInfo.width) - halfLength;
			m_Vertices[i + 1] = 0.f;
			m_Vertices[i + 2] = (static_cast<float>(z) / sideVertCount * createInfo.width) - halfLength;

			// UV
			m_Vertices[i + 3] = (static_cast<float>(x) / sideVertCount);
			m_Vertices[i + 4] = (static_cast<float>(z) / sideVertCount);

			// NORMAL
			m_Vertices[i + 5] = 0.f;
			m_Vertices[i + 6] = 1.f;
			m_Vertices[i + 7] = 0.f;
		}
	}

	m_Indices.resize(sideVertCount * sideVertCount * 6);

	for (int ti = 0, vi = 0, x = 0; x < sideVertCount; ++vi, ++x)
	{
		for (int z = 0; z < sideVertCount; ti += 6, ++vi, ++z)
		{
			m_Indices[ti] = vi;
			m_Indices[ti + 1] = vi + 1;
			m_Indices[ti + 2] = vi + sideVertCount + 2;
			m_Indices[ti + 3] = vi;
			m_Indices[ti + 4] = vi + sideVertCount + 2;
			m_Indices[ti + 5] = vi + sideVertCount + 1;
		}
	}

	m_VertexBuffer.UploadData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
	m_IndexBuffer.UploadData(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));

	VertexArray::AttribInfo positionAttrib{ 3, GL_FLOAT, 0, 0 };
	VertexArray::AttribInfo uvAttrib{ 2, GL_FLOAT, sizeof(float) * 3, 2 };
	VertexArray::AttribInfo normalAttrib{ 3, GL_FLOAT, sizeof(float) * 5, 1 };

	std::array<VertexArray::AttribInfo, 3> attribs = {
		positionAttrib, uvAttrib, normalAttrib
	};
	VertexArray::BufferInfo vertexBufferInfo
	{
		m_VertexBuffer, 0, sizeof(float) * 8, 0, attribs
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
}

void Plane::Draw(DrawInfo drawInfo)
{
	m_Shader.Bind();
	m_Shader.setMat4("proj", drawInfo.proj);
	m_Shader.setMat4("view", drawInfo.view);
	m_Shader.setF("u_Time", glfwGetTime());

	glm::mat4 model{ 1.0f };
	model = glm::translate(model, transform.pos);
	model = glm::rotate(model, transform.rot.x, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, transform.rot.y, glm::vec3(-1.f, 0.f, 0.f));
	model = glm::rotate(model, transform.rot.z, glm::vec3(0.f, 0.f, -1.f));
	model = glm::scale(model, transform.scale);

	m_Shader.setMat4("model", model);

	if (m_Texture.getHandle() != 69)
	{
		m_Texture.bind();
	}

	m_Mesh.Draw(m_Indices.size());
}