
/// Internal Includes
#include "AssetManager.hpp"
#include "Formats/Formats.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Mesh/Mesh.hpp"

/// External Includes
#include <AssetLib/AssetLib.hpp>
#include <glm/glm.hpp>

/// Std Includes
#include <vector>

namespace Engine {

	Interfaces::ICMesh* AssetManager::loadMesh(const char* model) {

		std::string mdl = "";

		if (gAssetDataPath) {
			mdl = gAssetDataPath;
		}

		mdl += model;

		if (loadedAssets.count(mdl) > 0) {
			return loadedAssets[mdl];
		}

		uint32_t size = 0;
		void* data = AssetLib::fileToMemory(mdl.c_str(), size);

		Interfaces::ICMesh* mesh = nullptr;

		if (data) {
			Core::MemoryBuffer memBuff;
			memBuff.setData(data, size);
			delete data;

			Formats::GenericHeader header = *memBuff.returnBytes<Formats::GenericHeader>(sizeof(Formats::GenericHeader));

			if (header.version == 0x0100) {
				mesh = loadMeshV2_0(memBuff);
			}

			memBuff.deleteBuffer();
		}

		if (mesh) {
			loadedAssets[mdl] = mesh;
		}

		return mesh;
	}

	Interfaces::ICMesh* AssetManager::loadMeshV2_0(Core::MemoryBuffer& memBuff) {

		typedef Formats::ModelFormat::version_2_0::MeshInfo MI;
		typedef Formats::ModelFormat::version_2_0::BoneInfo BI;
		typedef Formats::ModelFormat::version_2_0::KeyFrame KF;
		typedef Formats::ModelFormat::version_2_0::AnimationInfo AI;

		MI mi = *memBuff.returnBytes<MI>(sizeof(MI));
		bool uv = (mi.meshFlags & Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_UV) == Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_UV;
		bool normal = (mi.meshFlags & Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_NORMALS) == Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_NORMALS;
		bool vColors = (mi.meshFlags & Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_VCOLORS) == Formats::ModelFormat::version_2_0::MESH_INFO_FLAG_USE_VCOLORS;

		glm::vec3* positions = memBuff.returnBytes<glm::vec3>(mi.numVerts * sizeof(glm::vec3));

		glm::vec3* normals = normal ? memBuff.returnBytes<glm::vec3>(sizeof(glm::vec3) * mi.numVerts) : nullptr;
		glm::vec4* colors = vColors ? memBuff.returnBytes<glm::vec4>(sizeof(glm::vec4) * mi.numVerts) : nullptr;
		glm::vec2* uvs = uv ? memBuff.returnBytes<glm::vec2>(sizeof(glm::vec2) * mi.numVerts) : nullptr;

		glm::vec4* weights = (mi.numBones > 0) ? memBuff.returnBytes<glm::vec4>(sizeof(glm::vec4) * mi.numVerts) : nullptr;
		glm::ivec4* wIndex = (mi.numBones > 0) ? memBuff.returnBytes<glm::ivec4>(sizeof(glm::ivec4) * mi.numVerts) : nullptr;

		Formats::Triangle* triangles = (Formats::Triangle*)memBuff.returnBytes<Formats::Triangle>(sizeof(Formats::Triangle) * mi.numTris);

		BI* bi = (mi.numBones > 0) ? memBuff.returnBytes<BI>(sizeof(BI) * mi.numBones) : nullptr;

		// worst case, can be optimized;
		uint32_t size = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec4);
		size *= (mi.numTris * 3);
		void* data = malloc(size);

		Core::MemoryPusher memPush;
		memPush.setBuffer(data, size);

		auto fn = [&](uint32_t idx) {
			memPush.pushData(&positions[idx], sizeof(glm::vec3));
			if (uv) {
				memPush.pushData(&uvs[idx], sizeof(glm::vec2));
			}
			if (normal) {
				memPush.pushData(&normals[idx], sizeof(glm::vec3));
			}
			if (vColors) {
				memPush.pushData(&colors[idx], sizeof(glm::vec4));
			}
			if (mi.numBones > 0) {
				memPush.pushData(&weights[idx], sizeof(glm::vec4));
				memPush.pushData(&wIndex[idx], sizeof(glm::vec4));
			}
		};

		for (size_t i = 0; i < mi.numTris; i++) {
			Formats::Triangle t = triangles[i];

			fn(t.v1);
			fn(t.v2);
			fn(t.v3);
		}

		short count = 0;
		DataLayout layout[6];

		for (size_t i = 0; i < 6; i++) {
			layout[i].dataType = LayoutDataType::INVALID;
		}

		auto fn2 = [&](int idx, LayoutDataType elemCnt) {
			layout[idx].dataType = elemCnt;
			layout[idx].offset = 0;
		};

		fn2(count, LayoutDataType::POSITION);
		count++;
		if (uv) {
			fn2(count, LayoutDataType::UV);
			count++;
		}
		if (normal) {
			fn2(count, LayoutDataType::NORMAL);
			count++;
		}
		if (vColors) {
			fn2(count, LayoutDataType::COLOR);
			count++;
		}
		if (mi.numBones > 0) {
			fn2(count, LayoutDataType::WEIGHT);
			count++;
			fn2(count, LayoutDataType::INDEX);
			count++;
		}

		Graphics::Mesh::CMesh* mesh = new Graphics::Mesh::CMesh();

		IMesh* mdl = mesh->getIMesh();

		mdl->setMeshData(data, memPush.getOffset(), layout, count + 1);

		delete data;

		return mesh;
	}

}