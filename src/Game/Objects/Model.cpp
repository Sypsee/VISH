#include "Model.h"

#include <iostream>
#include <stb_image/stb_image.h>

Model::Model(std::filesystem::path path)
{
	auto parser = fastgltf::Parser(fastgltf::Extensions::KHR_texture_basisu | fastgltf::Extensions::KHR_mesh_quantization |
		fastgltf::Extensions::EXT_meshopt_compression | fastgltf::Extensions::KHR_lights_punctual);

	auto data = fastgltf::MappedGltfFile::FromPath(path);

	if (!data)
	{
		std::cerr << "Failed to open glTF file: " << fastgltf::getErrorMessage(data.error()) << '\n';
	}

	constexpr auto options = fastgltf::Options::LoadExternalBuffers | fastgltf::Options::LoadExternalImages |
		fastgltf::Options::LoadGLBBuffers;

	auto asset = parser.loadGltf(data.get(), path.parent_path(), options);

	for (auto& image : asset->images)
	{
		if (!loadImage(asset.get(), image))
		{
			std::cerr << "Failed to load mesh images!\n";
		}
	}

	m_Meshes.resize(asset->meshes.size());
	int i = 0;
	for (auto& mesh : asset->meshes)
	{
		if (!loadMesh(asset.get(), mesh, i))
		{
			std::cerr << "Failed to load mesh!\n";
		}
		i++;
	}

	m_Shader.AttachShader({ "res/shaders/lit.vert", GL_VERTEX_SHADER });
	m_Shader.AttachShader({ "res/shaders/lit.frag", GL_FRAGMENT_SHADER });
	m_Shader.setI("u_HasTex", false);
}

Model::~Model()
{
	m_Meshes.clear();
	m_Textures.clear();
}

bool Model::loadMesh(fastgltf::Asset& asset, fastgltf::Mesh &mesh, const int i)
{
	for (auto it = mesh.primitives.begin(); it != mesh.primitives.end(); ++it)
	{
		std::vector<Vertex> vertices;

		auto& positionAccessor = asset.accessors[it->findAttribute("POSITION")->accessorIndex];

		vertices.resize(positionAccessor.count);
		fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {
			vertices[idx].position = glm::vec3(pos.x(), pos.y(), pos.z());
		});

		std::size_t baseColorTexcoordIndex = 0;

		if (it->materialIndex.has_value())
		{
			auto& material = asset.materials[it->materialIndex.value()];
			auto& baseColorTexture = material.pbrData.baseColorTexture;

			if (baseColorTexture.has_value())
			{
				auto& texture = asset.textures[baseColorTexture->textureIndex];
				if (!texture.imageIndex.has_value())
					return false;

				m_Meshes[i].albedoTextureIndex = texture.imageIndex.value();
				
				if (baseColorTexture->transform && baseColorTexture->transform->texCoordIndex.has_value())
				{
					baseColorTexcoordIndex = baseColorTexture->transform->texCoordIndex.value();
				}
				else
				{
					baseColorTexcoordIndex = material.pbrData.baseColorTexture->texCoordIndex;
				}
			}
		}

		auto texCoordAttrib = std::string("TEXCOORD_") + std::to_string(baseColorTexcoordIndex);
		if (const auto* texCoord = it->findAttribute(texCoordAttrib); texCoord != it->attributes.end())
		{
			auto& texCoordAccessor = asset.accessors[texCoord->accessorIndex];

			if (!texCoordAccessor.bufferViewIndex.has_value())
				continue;

			fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, texCoordAccessor, [&](fastgltf::math::fvec2 uv, std::size_t idx) {
				vertices[idx].uv = glm::vec2(uv.x(), uv.y());
			});
		}

		auto& indexAccessor = asset.accessors[it->indicesAccessor.value()];

		GLenum indexType = GL_NONE;
		if (indexAccessor.componentType == fastgltf::ComponentType::UnsignedByte || indexAccessor.componentType == fastgltf::ComponentType::UnsignedShort)
		{
			indexType = GL_UNSIGNED_SHORT;
			std::vector<uint16_t> indices;
			indices.resize(indexAccessor.count);
			fastgltf::copyFromAccessor<std::uint16_t>(asset, indexAccessor, indices.data());
			m_Meshes[i].indexBuffer.UploadData(&indices[0], indices.size() * sizeof(uint16_t));

			m_Meshes[i].indexCount = indices.size();
		}
		else
		{
			indexType = GL_UNSIGNED_INT;
			std::vector<uint32_t> indices;
			indices.resize(indexAccessor.count);
			fastgltf::copyFromAccessor<std::uint32_t>(asset, indexAccessor, indices.data());
			m_Meshes[i].indexBuffer.UploadData(&indices[0], indices.size() * sizeof(uint32_t));

			m_Meshes[i].indexCount = indices.size();
			indices.clear();
		}
		
		m_Meshes[i].vertexBuffer.UploadData(&vertices[0], vertices.size() * sizeof(Vertex));
		VertexArray::AttribInfo positionAttrib{ 3, GL_FLOAT, 0, 0 };
		VertexArray::AttribInfo uvAttrib{ 2, GL_FLOAT, 3 * sizeof(float), 2 };

		std::array<VertexArray::AttribInfo, 2> attribs = {
			positionAttrib, uvAttrib
		};
		VertexArray::BufferInfo vertexBufferInfo
		{
			m_Meshes[i].vertexBuffer, 0, sizeof(Vertex), 0, attribs
		};
		VertexArray::BufferInfo vertexBuffers[] = { vertexBufferInfo };
		VertexArray::CreateInfo vaCreateInfo
		{
			std::span<VertexArray::BufferInfo>(std::move(vertexBuffers), 1), true, std::move(m_Meshes[i].indexBuffer)
		};
		VertexArray va{ vaCreateInfo };

		Mesh::CreateInfo meshCreateInfo
		{
			std::move(va), indexType
		};
		m_Meshes[i].mesh = Mesh(meshCreateInfo);

		vertices.clear();
	}

	return true;
}

bool Model::loadImage(fastgltf::Asset& asset, fastgltf::Image& image)
{
	auto getLevelCount = [](int width, int height) -> GLsizei {
		return static_cast<GLsizei>(1 + floor(log2(width > height ? width : height)));
	};

	unsigned int texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);

	std::visit(fastgltf::visitor
	{
		[](auto& arg) {},
		[&](fastgltf::sources::URI& filePath) {
			assert(filePath.fileByteOffset == 0);
			assert(filePath.uri.isLocalPath());
			int width, height, nrChannels;

			const std::string path(filePath.uri.path().begin(), filePath.uri.path().end());
			unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
			glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
			glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		},
		[&](fastgltf::sources::Array& vector) {
			int width, height, nrChannels;
			unsigned char* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(vector.bytes.data()), static_cast<int>(vector.bytes.size()), &width, &height, &nrChannels, 4);
			glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
			glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		},
		[&](fastgltf::sources::BufferView& view) {
			auto& bufferView = asset.bufferViews[view.bufferViewIndex];
			auto& buffer = asset.buffers[bufferView.bufferIndex];
			std::visit(fastgltf::visitor {
				[](auto& arg) {},
				[&](fastgltf::sources::Array& vector) {
					int width, height, nrChannels;
					unsigned char* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(vector.bytes.data() + bufferView.byteOffset),
																static_cast<int>(bufferView.byteLength), &width, &height, &nrChannels, 4);
					glTextureStorage2D(texture, getLevelCount(width, height), GL_RGBA8, width, height);
					glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
					stbi_image_free(data);
				}
			}, buffer.data);
		},
	}, image.data);

	glGenerateTextureMipmap(texture);
	m_Textures.push_back(texture);

	return true;
}

void Model::Draw(DrawInfo drawInfo)
{
	m_Shader.Bind();
	m_Shader.setMat4("proj", drawInfo.proj);
	m_Shader.setMat4("view", drawInfo.view);

	glm::mat4 model(1.0);
	//model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
	m_Shader.setMat4("model", model);
	m_Shader.setVec3("viewPos", drawInfo.viewPos);

	m_Shader.setI("u_LightsSize", drawInfo.lights.size());

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

	for (ModelMesh& m : m_Meshes)
	{
		if (m_Textures.size() > 0)
		{
			m_Shader.setI("u_HasTex", true);
			glBindTextureUnit(1, m_Textures[m.albedoTextureIndex]);
			m_Shader.setI("u_Tex", 1);
		}

		m.mesh.Draw(m.indexCount);
	}
}