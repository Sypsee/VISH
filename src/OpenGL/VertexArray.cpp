#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray(CreateInfo const& createInfo)
{
	glCreateVertexArrays(1, &m_Handle);

	for (int i = 0; i < createInfo.buffers.size(); i++)
	{
		const BufferInfo& bufferInfo = createInfo.buffers[i];
		
		glVertexArrayVertexBuffer(m_Handle, i, bufferInfo.buffer.getHandle(), bufferInfo.offset, bufferInfo.stride);

		for (int j = 0; j < bufferInfo.attribInfo.size(); j++)
		{
			const AttribInfo& attribInfo = createInfo.buffers[i].attribInfo[j];

			glVertexArrayAttribBinding(m_Handle, attribInfo.layoutIndex, bufferInfo.bindingIndex);
			glEnableVertexArrayAttrib(m_Handle, attribInfo.layoutIndex);
			glVertexArrayAttribFormat(m_Handle, attribInfo.layoutIndex, attribInfo.atSize, attribInfo.atType, GL_FALSE, attribInfo.offset);
		}
	}

	if (createInfo.hasIndexBuffer)
	{
		glVertexArrayElementBuffer(m_Handle, createInfo.indexBuffer.getHandle());
	}
}

VertexArray::~VertexArray() noexcept
{
	Destroy();
}

void VertexArray::Destroy()
{
	if (!m_Handle) return;

	glDeleteVertexArrays(1, &m_Handle);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Handle);
}