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
	m_Shader.setI("g_Albedo", 0);
	m_Shader.setI("g_Position", 1);
	m_Shader.setI("g_Normal", 2);
	m_Shader.setI("u_DepthTex", 3);
}

void Composite::Draw(DrawInfo drawInfo)
{
	if (!drawInfo.writeDepth)
	{
		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);
	}

	m_Shader.Bind();

	m_Shader.setMat4("u_ViewMatrix", drawInfo.viewMat);
	m_Shader.setMat4("u_ProjMatrix", drawInfo.projMat);
	m_Shader.setVec3("u_ViewPos", drawInfo.camPos);

	if (drawInfo.lights.size() > 0)
	{
		m_Shader.setI("u_LightsSize", static_cast<int>(drawInfo.lights.size()));
		int i = 0;
		for (const Light& light : drawInfo.lights)
		{
			std::string lightLoc = "u_Lights[" + std::to_string(i) + "]";
			std::string lightPosLoc = lightLoc + ".pos";
			std::string lightColorLoc = lightLoc + ".color";
			m_Shader.setVec3(lightPosLoc.c_str(), light.pos);
			m_Shader.setVec3(lightColorLoc.c_str(), light.color);
			i++;
		}
	}

	m_Mesh.Draw(6);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
}