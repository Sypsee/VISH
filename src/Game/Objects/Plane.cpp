#include "Plane.h"

#include <array>
#include <string>

Plane::Plane(CreateInfo const& createInfo)
{
	float triangleSide = createInfo.width / createInfo.resolution;

	for (int row = 0; row < createInfo.resolution + 1; row++)
	{
		for (int col = 0; col < createInfo.resolution + 1; col++)
		{
			glm::vec3 crntVec = glm::vec3(col * triangleSide, 0.0, row * -triangleSide);
			m_Vertices.push_back(crntVec.x);
			m_Vertices.push_back(crntVec.y);
			m_Vertices.push_back(crntVec.z);

			if (row < createInfo.resolution && col < createInfo.resolution)
			{
				int index = row * (createInfo.resolution + 1) + col;

				m_Indices.push_back(index);
				m_Indices.push_back(index + (createInfo.resolution + 1) + 1);
				m_Indices.push_back(index + (createInfo.resolution + 1));

				m_Indices.push_back(index);
				m_Indices.push_back(index + 1);
				m_Indices.push_back(index + (createInfo.resolution + 1) + 1);
			}
		}
	}

	m_VertexBuffer.UploadData(m_Vertices.data(), m_Vertices.size() * sizeof(float));
	m_IndexBuffer.UploadData(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));

	VertexArray::AttribInfo positionAttrib{ 3, GL_FLOAT, 0, 0 };
	//VertexArray::AttribInfo uvAttrib{ 2, GL_FLOAT, sizeof(float) * 3, 2 };
	//VertexArray::AttribInfo normalAttrib{ 3, GL_FLOAT, sizeof(float) * 3, 1 };

	std::array<VertexArray::AttribInfo, 1> attribs = {
		positionAttrib
	};
	VertexArray::BufferInfo vertexBufferInfo
	{
		m_VertexBuffer, 0, sizeof(float) * 3, 0, attribs
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

	m_Shader.AttachShader({ "res/shaders/lit.vert", GL_VERTEX_SHADER });
	m_Shader.AttachShader({ "res/shaders/lit.frag", GL_FRAGMENT_SHADER });

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