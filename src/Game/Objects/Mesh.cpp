#include "Mesh.h"

Mesh::Mesh(CreateInfo const& info)
	:m_Type(info.type)
{
	m_Va = std::move(info.va);
}

void Mesh::Draw(uint16_t m_TriCount)
{
	m_Va.Bind();

	if (m_Type != NULL)
	{
		glDrawElements(GL_TRIANGLES, m_TriCount, m_Type, NULL);
		return;
	}

	glDrawArrays(GL_TRIANGLES, 0, m_TriCount);
}