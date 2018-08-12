
/// Internal Includes
#include "AssetManager.hpp"
#include "Formats/Formats.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/Mesh/Mesh.hpp"

/// External Includes
#include <AssetLib/AssetLib.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// Std Includes
#include <functional>
#include <vector>

namespace Engine {

	typedef Graphics::Mesh::Animation::BindPoseInfo BindPoseInfo;

	AssetManager::~AssetManager() {

		auto asit = loadedAssets.begin();
		auto aseit = loadedAssets.end();

		while (asit != aseit) {

			if (asit->second) {
				delete asit->second;
				asit->second = nullptr;
			}
			asit++;
		}

		auto tit = loadedTextures.begin();
		auto teit = loadedTextures.end();

		while (tit != teit) {

			if (tit->second) {
				delete tit->second;
				tit->second = nullptr;
			}
			tit++;
		}

	}

	Interfaces::ICMesh* AssetManager::loadMesh(const char* model) {

		std::string mdl = "";

		if (gAssetDataPath) {
			mdl = gAssetDataPath;
		}

		mdl += model;

		if (loadedAssets.count(mdl) > 0) {
			return loadedAssets[mdl];
		}

		Interfaces::ICMesh* mesh = nullptr;

		mesh = loadMeshV2_0(mdl.c_str());

		if (mesh) {
			loadedAssets[mdl] = mesh;
		}

		return mesh;
	}

	Graphics::Texture::Texture2D* AssetManager::loadTexture(const char* texture) {

		std::string tex = "";

		if (gAssetDataPath) {
			tex = gAssetDataPath;
		}

		tex += texture;


		if (loadedTextures.count(tex) > 0) {
			return loadedTextures[tex];
		}

		Graphics::Texture::Texture2D* t = new Graphics::Texture::Texture2D();

		int32_t x, y, c;

		x = y = c = 0;

		void* data = AssetLib::loadTexture(tex.c_str(), x, y, c);
		t->setData(x, y, c, data);

		AssetLib::freeImageData(data);


		if (t) {
			loadedTextures[tex] = t;
		}

		return t;
	}

	bool AssetManager::isLoaded(uint32_t entryId) {

		bool loaded = false;

		if (loadedEntries.count(entryId) > 0) {
			Entry e = loadedEntries[entryId];
			if (nullptr != e.data) {
				loaded = true;
			}
		}

		return loaded;
	}

	void AssetManager::cacheEntry(uint32_t entryId, Entry dataPtr) {
		loadedEntries[entryId] = dataPtr;
		dataPtr.refCounter = 1;
	}

	Entry AssetManager::getCachedEntry(uint32_t entryId) {
		Entry data;
		if (loadedEntries.count(entryId) > 0) {
			loadedEntries[entryId].refCounter++;
			data = loadedEntries[entryId];
		}
		return data;
	}

	//void AssetManager::incReferenceCount(uint32_t entryId) {
	//	if (loadedEntries.count(entryId) > 0) {
	//		loadedEntries[entryId].refCounter++;
	//	}
	//}

	void AssetManager::decRerferenceCount(uint32_t entryId) {
		if (loadedEntries.count(entryId) > 0) {
			loadedEntries[entryId].refCounter--;
		}
	}

	// static functions


	void deleteBindposeChilds(BindPoseInfo* info) {

		for (size_t i = 0; i < info->numChildren; i++) {
			deleteBindposeChilds(&info->children[i]);
		}

		delete[] info->children;

	};


	BindPoseInfo createBoneHierarchy(AssetLib::SkeletonBone* org, bool bindPose, AssetLib::JointKeyFrames* frames, AssetLib::Model &mdl) {

		BindPoseInfo b;

		b.boneIndex = org->boneIndex;

		size_t len = strlen(org->boneName);

		if (bindPose) {
			b.transformMatrix = AssetLib::toType<glm::mat4>(org->transform);
		} else {

			int index = 0;

			for (size_t i = 0; i < mdl.numBones; i++) {
				if (frames[i].boneIndex == org->boneIndex) {
					b.transformMatrix = AssetLib::toType<glm::mat4>(frames[i].transforms[17].transform);
				}
			}
		}

		b.numChildren = org->numChildren;

		if (org->children) {

			b.children = new BindPoseInfo[b.numChildren];

			for (size_t i = 0; i < b.numChildren; i++) {
				b.children[i] = createBoneHierarchy(&org->children[i], bindPose, frames, mdl);
			}
		} else {
			b.children = nullptr;
		}

		return b;


	}

	// private functions

	Interfaces::ICMesh* AssetManager::loadMeshV2_0(const char* file) {

		AssetLib::Model mdl = AssetLib::loadModel(file);

		uint32_t size = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec4);
		size *= (mdl.numFaces * 3);
		void* data = malloc(size);

		Core::MemoryPusher memPush;
		memPush.setBuffer(data, size);

		auto fn = [&](uint32_t idx) {
			glm::vec3 pos = AssetLib::toType<glm::vec3>(mdl.verices[idx]);

			memPush.pushData(&pos, sizeof(glm::vec3));
			if (mdl.hasUv) {
				memPush.pushData(&mdl.uvs[idx], sizeof(glm::vec2));
			}
			if (mdl.hasNormals) {
				memPush.pushData(&mdl.normals[idx], sizeof(glm::vec3));
			}
			if (mdl.hasColors) {
				memPush.pushData(&mdl.vertexColors[idx], sizeof(glm::vec4));
			}
			if (mdl.hasWeights) {
				memPush.pushData(&mdl.vertexWeights[idx], sizeof(glm::vec4));
				memPush.pushData(&mdl.vertexIndex[idx], sizeof(glm::vec4));
			}
		};

		for (size_t i = 0; i < mdl.numFaces; i++) {
			AssetLib::Triangle t = mdl.faces[i];

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
		if (mdl.hasUv) {
			fn2(count, LayoutDataType::UV);
			count++;
		}
		if (mdl.hasNormals) {
			fn2(count, LayoutDataType::NORMAL);
			count++;
		}
		if (mdl.hasColors) {
			fn2(count, LayoutDataType::COLOR);
			count++;
		}
		if (mdl.hasWeights) {
			fn2(count, LayoutDataType::WEIGHT);
			count++;
			fn2(count, LayoutDataType::INDEX);
			count++;
		}

		Graphics::Mesh::CMesh* mesh = new Graphics::Mesh::CMesh();

		IMesh* meshData = mesh->getIMesh();

		meshData->setMeshData(data, memPush.getOffset(), layout, count + 1);

		if (mdl.skeleton) {

			AssetLib::JointKeyFrames* frames = mdl.animations[0].joint;

			std::function<BindPoseInfo(AssetLib::SkeletonBone*, bool)> copy = [&](AssetLib::SkeletonBone* org, bool other) -> BindPoseInfo {

				BindPoseInfo b;

				b.boneIndex = org->boneIndex;

				size_t len = strlen(org->boneName);

				if (!other) {
					b.transformMatrix = AssetLib::toType<glm::mat4>(org->transform);
				} else {

					int index = 0;

					for (size_t i = 0; i < mdl.numBones; i++) {
						if (frames[i].boneIndex == org->boneIndex) {
							b.transformMatrix = AssetLib::toType<glm::mat4>(frames[i].transforms[0].transform);
						}
					}
				}

				b.numChildren = org->numChildren;

				if (org->children) {

					b.children = new BindPoseInfo[b.numChildren];

					for (size_t i = 0; i < b.numChildren; i++) {
						b.children[i] = copy(&org->children[i], other);
					}
				} else {
					b.children = nullptr;
				}

				return b;
			};

			uint32_t numBones = mdl.numBones;
			BindPoseInfo* poseInfo = new BindPoseInfo;

			bool bindPose = true;
			*poseInfo = createBoneHierarchy(mdl.skeleton, bindPose, frames, mdl);

			Graphics::Mesh::Animation* anim = new Graphics::Mesh::Animation;

			anim->setBindPoseData(poseInfo, numBones);

			deleteBindposeChilds(poseInfo);

			bindPose = false;
			*poseInfo = createBoneHierarchy(mdl.skeleton, bindPose, frames, mdl);

			anim->setRawAnim(poseInfo);

			deleteBindposeChilds(poseInfo);

			delete poseInfo;

			mesh->setAnimationData(anim);

		}

		free(data);

		return mesh;
	}


}