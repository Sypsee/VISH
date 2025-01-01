#pragma once

#include "Mesh.h"
#include "../../OpenGL/Shader.h"
#include "../../OpenGL/Texture.h"
#include "ObjectInfo.h"

#include <fastgltf/core.hpp>
#include <fastgltf/tools.hpp>
#include <fastgltf/types.hpp>

#include <vector>
#include <filesystem>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

struct ModelMesh
{
	uint32_t indexCount = 0;
	uint8_t albedoTextureIndex = 0;
	Buffer vertexBuffer{ {GL_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW} };
	Buffer indexBuffer{ {GL_ELEMENT_ARRAY_BUFFER, NULL, NULL, GL_STATIC_DRAW} };
	fastgltf::math::fmat4x4 modelMatrix;
	Mesh mesh{};
};

class Model
{
public:
	Model(std::filesystem::path path);
	~Model();

	struct DrawInfo
	{
		glm::mat4 proj{ 1.0 };
		glm::mat4 view{ 1.0 };
		glm::vec3 viewPos{ 0.0 };

		std::span<Light> lights;
	};

	void Draw(DrawInfo drawInfo);

private:
	bool loadMesh(fastgltf::Asset& asset, fastgltf::Mesh& mesh, const int i);
	bool loadImage(fastgltf::Asset& asset, fastgltf::Image& image);

	std::vector<ModelMesh> m_Meshes;
	std::vector<unsigned int> m_Textures;

	Shader m_Shader;
};