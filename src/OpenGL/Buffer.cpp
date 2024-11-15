#include "Buffer.h"

Buffer::Buffer(CreateInfo const& createInfo)
	:m_Target(createInfo.target), m_Usage(createInfo.usage)
{
	glCreateBuffers(1, &m_BufferID);
	glNamedBufferData(m_BufferID, createInfo.size, createInfo.data, createInfo.usage);
}

Buffer::~Buffer() noexcept
{
	Destroy();
}

void Buffer::UploadData(const void* data, size_t size)
{
	Bind();
	glNamedBufferData(m_BufferID, size, data, m_Usage);
}

void Buffer::Destroy()
{
	glDeleteBuffers(1, &m_BufferID);
}

void Buffer::Bind() const
{
	glBindBuffer(m_Target, m_BufferID);
}

void Buffer::UnBind() const
{
	glBindBuffer(m_Target, 0);
}