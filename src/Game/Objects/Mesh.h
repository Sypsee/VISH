#pragma once

#include "../../OpenGL/VertexArray.h"

class Mesh final
{
public:
	struct CreateInfo final
	{
		VertexArray&& va;
		GLenum type = NULL;
	};

	constexpr Mesh() noexcept = default;
	Mesh(CreateInfo const& info);

	void Draw(uint16_t m_TriCount);

private:
	VertexArray m_Va;
	GLenum m_Type=NULL;
};