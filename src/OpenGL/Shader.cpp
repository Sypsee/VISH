#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void CheckCompileError(unsigned int shader, const char* shaderType)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		int maxLength = 512;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        std::cout << shaderType << " : COMPILATION ERROR : " << std::endl;

        for (char log : errorLog)
        {
            std::cout << log;
        }

        std::cout << std::endl;

		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.
	}
}


Shader::Shader()
{
    m_ProgramID = glCreateProgram();
}

Shader::~Shader() noexcept
{
	DestroyShader();
}

void Shader::AttachShader(AttachInfo const& attachInfo)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(attachInfo.shaderPath);
		std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Failed to read shader file!" << std::endl;
	}
	const char* shaderCodeChar = shaderCode.c_str();

	unsigned int shader = glCreateShader(attachInfo.shaderType);
	glShaderSource(shader, 1, &shaderCodeChar, NULL);
	glCompileShader(shader);

	CheckCompileError(shader, attachInfo.shaderPath);

	glAttachShader(m_ProgramID, shader);
	glLinkProgram(m_ProgramID);

    m_Shaders.emplace_back(shader);
}

void Shader::DestroyShader()
{
    for (auto& shader : m_Shaders)
    {
        glDeleteShader(shader);
    }
	glDeleteProgram(m_ProgramID);
}

void Shader::Bind() const
{
    glUseProgram(m_ProgramID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::setF4(const char* u_name, float v1, float v2, float v3, float v4)
{
	glUniform4f(GetUniformLocation(u_name), v1, v2, v3, v4);
}

void Shader::setF(const char* u_name, float v1)
{
    glUniform1f(GetUniformLocation(u_name), v1);
}

void Shader::setI(const char* u_name, float v1)
{
    glUniform1i(GetUniformLocation(u_name), v1);
}

void Shader::setVec3(const char* u_name, glm::vec3 val)
{
    glUniform3fv(GetUniformLocation(u_name), 1, glm::value_ptr(val));
}

void Shader::setVec2(const char* u_name, glm::vec2 val)
{
	glUniform2fv(GetUniformLocation(u_name), 1, glm::value_ptr(val));
}

void Shader::setMat4(const char* u_name, glm::mat4 val)
{
    glUniformMatrix4fv(GetUniformLocation(u_name), 1, GL_FALSE, glm::value_ptr(val));
}


unsigned int Shader::GetUniformLocation(const char* u_name)
{
    unsigned int loc = glGetUniformLocation(m_ProgramID, u_name);
    return loc;
}
