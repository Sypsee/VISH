#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Shader final
{
public:
	struct AttachInfo final
	{
		const char* shaderPath = 0;
		GLenum shaderType = 0;
	};

	//constexpr Shader() noexcept = delete;
	Shader();
	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;
	Shader& operator=(Shader&& other) noexcept
	{
		std::swap(m_ProgramID, other.m_ProgramID);
		return *this;
	}
	inline Shader(Shader&& other) noexcept { *this = std::move(other); }
	~Shader() noexcept;
	
	void AttachShader(AttachInfo const& attachInfo);
	void DestroyShader();

	void Bind() const;
	void UnBind() const;

	void setF4(const char* u_name, float v1, float v2, float v3, float v4);
	void setF(const char* u_name, float v1);
	void setI(const char* u_name, float v1);
	void setVec3(const char* u_name, glm::vec3 val);
	void setVec2(const char* u_name, glm::vec2 val);
	void setMat4(const char* u_name, glm::mat4 val);

private:
	unsigned int GetUniformLocation(const char* u_name);
	
    std::vector<unsigned int> m_Shaders{0};
    unsigned int m_ProgramID = 0;
};
