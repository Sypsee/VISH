#pragma once

#include "Buffer.h"
#include <span>
#include <optional>

class VertexArray final
{
public:
	struct AttribInfo final
	{
		size_t atSize = 0;
		GLenum atType = GL_FLOAT;
		int offset = 0;
		int layoutIndex = 0;
	};

	struct BufferInfo final
	{
		const Buffer& buffer;
		const uint8_t offset = 0;
		const uint8_t stride = 0;
		const uint8_t bindingIndex = 0;
		std::span<AttribInfo> attribInfo;
	};

	struct CreateInfo final
	{
		std::span<const BufferInfo> buffers;
		bool hasIndexBuffer = false;
		const Buffer& indexBuffer;
	};

	constexpr VertexArray() noexcept = default;
	VertexArray(CreateInfo const& createInfo);
	VertexArray(VertexArray const&) = delete;
	VertexArray& operator=(VertexArray const&) = delete;
	VertexArray& operator=(VertexArray&& other) noexcept
	{
		std::swap(m_Handle, other.m_Handle);
		return *this;
	}
	inline VertexArray(VertexArray&& other) noexcept { *this = std::move(other); }
	~VertexArray() noexcept;

	void Destroy();
	void Bind() const;

	inline unsigned int getHandle() const { return m_Handle; }

private:
	unsigned int m_Handle=0;
};