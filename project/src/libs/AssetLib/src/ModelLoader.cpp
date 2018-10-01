#include <AssetLib/AssetLib.hpp>
#include "LoaderHelpers.hpp"

#include "CustomMesh/MeshFormat.hpp"

//#include <TinyXml/tinyxml2.h>

#include "Collada/ColladaLoader.hpp"

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <iterator>
#include <functional>

namespace AssetLib {

	bool endsWithCaseInsensitive(std::string mainStr, std::string toMatch) {
		auto it = toMatch.begin();
		return mainStr.size() >= toMatch.size() &&
			std::all_of(std::next(mainStr.begin(), mainStr.size() - toMatch.size()), mainStr.end(), [&it](const char & c) {
			return ::tolower(c) == ::tolower(*(it++));
		});
	}

	SkeletonBone makeSkeletonBone(Format::version_2_0::BoneInfo* boneInfo, Format::version_2_0::Name* name) {

		SkeletonBone bone;
		bone.boneIndex = boneInfo->boneId;
		memcpy(&bone.transform, boneInfo->localBindPose, sizeof(Matrix4x4));
		char* nn = new char[static_cast<uint64_t>(name->length) + 1];
		memcpy(nn, name->name, name->length);
		nn[name->length] = '\0';
		bone.boneName = nn;

		bone.children = nullptr;
		bone.numChildren = 0U;

		return bone;
	}

	void createFromMesh(Model &mdl, const char* fileName) {

		typedef Format::Header::GenericHeader Head;
		typedef Format::version_2_0::MeshInfo MI;
		typedef Format::version_2_0::BoneInfo BI;
		typedef Format::version_2_0::AnimationInfo AI;
		typedef Format::version_2_0::KeyFrame KF;

		uint32_t offset = 0;
		uint32_t dataSize = 0;

		void* data = fileToMemory(fileName, dataSize);

		Head header = *(Head*)dataAtOffset(data, offset, sizeof(Head), dataSize);

		if (header.version == 0x0100) {

			MI mi = *(MI*)dataAtOffset(data, offset, sizeof(MI), dataSize);

			mdl.numVertices = mi.numVerts;

			bool uv = (mi.meshFlags & Format::version_2_0::MESH_INFO_FLAG_USE_UV) == Format::version_2_0::MESH_INFO_FLAG_USE_UV;
			bool normal = (mi.meshFlags & Format::version_2_0::MESH_INFO_FLAG_USE_NORMALS) == Format::version_2_0::MESH_INFO_FLAG_USE_NORMALS;
			bool vColors = (mi.meshFlags & Format::version_2_0::MESH_INFO_FLAG_USE_VCOLORS) == Format::version_2_0::MESH_INFO_FLAG_USE_VCOLORS;
			bool vWeights = mi.numBones > 0;

			mdl.hasUv = uv;
			mdl.hasNormals = normal;
			mdl.hasColors = vColors;
			mdl.hasWeights = vWeights;

			mdl.verices = new AssetLib::Vector3f[mdl.numVertices];
			void* vData = dataAtOffset(data, offset, sizeof(Vector3f) * mdl.numVertices, dataSize);
			memcpy(mdl.verices, vData, sizeof(Vector3f) * mdl.numVertices);

			if (normal) {
				mdl.normals = new AssetLib::Vector3f[mdl.numVertices];
				void* nData = dataAtOffset(data, offset, sizeof(Vector3f) * mdl.numVertices, dataSize);
				memcpy(mdl.normals, nData, sizeof(Vector3f) * mdl.numVertices);
			}
			if (vColors) {
				mdl.vertexColors = new AssetLib::Color4f[mdl.numVertices];
				void* cData = dataAtOffset(data, offset, sizeof(Color4f) * mdl.numVertices, dataSize);
				memcpy(mdl.vertexColors, cData, sizeof(Color4f) * mdl.numVertices);
			}
			if (uv) {
				mdl.uvs = new AssetLib::Vector2f[mdl.numVertices];
				void* uData = dataAtOffset(data, offset, sizeof(Vector2f) * mdl.numVertices, dataSize);
				memcpy(mdl.uvs, uData, sizeof(Vector2f) * mdl.numVertices);
			}
			if (vWeights) {
				mdl.vertexWeights = new AssetLib::VertexBoneWeight[mdl.numVertices];
				mdl.vertexIndex = new AssetLib::VertexBoneIndex[mdl.numVertices];

				void* wData = dataAtOffset(data, offset, sizeof(VertexBoneWeight) * mdl.numVertices, dataSize);
				memcpy(mdl.vertexWeights, wData, sizeof(VertexBoneWeight) * mdl.numVertices);

				wData = dataAtOffset(data, offset, sizeof(VertexBoneIndex) * mdl.numVertices, dataSize);
				memcpy(mdl.vertexIndex, wData, sizeof(VertexBoneIndex) * mdl.numVertices);
			}

			mdl.numFaces = mi.numTris;

			mdl.faces = new Triangle[mdl.numFaces];
			void* tData = dataAtOffset(data, offset, sizeof(Triangle) * mdl.numFaces, dataSize);
			memcpy(mdl.faces, tData, sizeof(Triangle) * mdl.numFaces);


			if (mi.numBones > 0) {

				mdl.numBones = mi.numBones;
				Format::version_2_0::Name* names = new Format::version_2_0::Name[mi.numBones];
				
				for (uint8_t i = 0; i < mi.numBones; i++) {
				
					uint16_t nameLength = *(uint16_t*)dataAtOffset(data, offset, sizeof(uint16_t), dataSize);
				
					void* ndata = dataAtOffset(data, offset, sizeof(char) * nameLength, dataSize);
				
					names[i].length = nameLength;
					names[i].name = (char*)ndata;
				
				}
				
				BI* skeleton = (BI*)dataAtOffset(data, offset, sizeof(BI) * mi.numBones, dataSize);
				
				std::vector<SkeletonBone> bones;
				
				uint8_t rootIndex = 0U;
				
				for (uint8_t i = 0; i < mi.numBones; i++) {
					SkeletonBone b = makeSkeletonBone(&skeleton[i], &names[skeleton[i].nameIndex]);
					if (skeleton[i].boneId == skeleton[i].parentBone) {
						rootIndex = i;
					}
				
					bones.push_back(b);
				
				}
				
				mdl.skeleton = new SkeletonBone;
				
				*mdl.skeleton = bones[rootIndex];
				
				std::vector<SkeletonBone>::iterator it = bones.begin() + rootIndex;
				
				bones.erase(it);
				
				while (!bones.empty()) {
				
					for (size_t i = 0; i < bones.size(); i++) {
				
						uint16_t p = skeleton[bones[i].boneIndex].parentBone;
						bool ok = mdl.skeleton->addChild(bones[i], p);
				
						if (ok) {
							it = bones.begin() + i;
							bones.erase(it);
						}
				
					}
				}
				
				delete[] names;

				mdl.numAnimations = mi.numAnimations;
				mdl.animations = new Animation[mdl.numAnimations];
				
				for (uint16_t a = 0; a < mi.numAnimations; a++) {
					mdl.animations[a].animationName = "bla";
					
					mdl.animations[a].joint = nullptr;
					mdl.animations[a].numJoints = 0;
					
					mdl.animations[a].joint = new JointKeyFrames[mi.numBones];
					mdl.animations[a].numJoints = mi.numBones;
					
					AI* anims = (AI*)dataAtOffset(data, offset, sizeof(AI), dataSize);
					
					uint16_t numKeyFrames = anims->numKeyFrames;
					
					KF* keyFrames = (KF*)dataAtOffset(data, offset, sizeof(KF) * mi.numBones * numKeyFrames, dataSize);
					
					for (uint8_t b = 0; b < mi.numBones; b++) {
					
						mdl.animations[a].joint[b].numTransforms = numKeyFrames;
						mdl.animations[a].joint[b].triggerTime = 0;
						mdl.animations[a].joint[b].transforms = new JointTransform[numKeyFrames];
						mdl.animations[a].joint[b].boneIndex = keyFrames[b].boneId;
					
						Matrix4x4 m;
						for (uint16_t j = 0; j < numKeyFrames; j++) {
					
							uint16_t jointOffset = j * mi.numBones;
					
							memcpy(&m, keyFrames[jointOffset + b].transform, sizeof(Matrix4x4));
					
							mdl.animations[a].joint[b].transforms[j].transform = m;
						}
					}

				}

				//AI* anims = (AI*)dataAtOffset(data, offset, sizeof(AI), dataSize);
				//
				//uint16_t numKeyFrames = anims->numKeyFrames;
				//
				//KF* keyFrames = (KF*)dataAtOffset(data, offset, sizeof(KF) * mi.numBones * numKeyFrames, dataSize);
				//
				//mdl.animations[0].animationName = "Test";
				//mdl.animations[0].numJoints = mi.numBones;
				//mdl.animations[0].joint = new JointKeyFrames[mi.numBones];
				//
				//for (uint8_t i = 0; i < mi.numBones; i++) {
				//
				//	mdl.animations[0].joint[i].numTransforms = 1;
				//	mdl.animations[0].joint[i].triggerTime = 0;
				//	mdl.animations[0].joint[i].transforms = new JointTransform[mdl.animations[0].joint[i].numTransforms];
				//	mdl.animations[0].joint[i].boneIndex = keyFrames[i].boneId;
				//
				//	Matrix4x4 m;
				//	memcpy(&m, keyFrames[i].transform, sizeof(Matrix4x4));
				//
				//	mdl.animations[0].joint[i].transforms[0].transform = m;
				//	int b = 0;
				//}

			}

		}

		delete[] data;
	}

	void createFromDae(Model& , const char* ) {

		/*
		ColladaLoader loader;

		if (loader.load(fileName)) {

			uint32_t num = loader.numGeomentries();

			const char* id = loader.geometryId(0);

			uint32_t numVerts = 0U;
			uint32_t numNormals = 0U;
			uint32_t numUvs = 0U;
			uint32_t numColors = 0U;
			uint32_t numWeights = 0U;
			uint32_t numWInds = 0U;

			uint32_t numTris = 0U;
			uint32_t stride = 0U;
			StrideList* strideList = nullptr;

			float* vertices = loader.getVertices(id, numVerts);
			float* normals = loader.getNormals(id, numNormals);
			float* uvs = loader.getUvs(id, numUvs);
			float* colors = loader.getColors(id, numColors);

			float* weights = loader.getWeights(id, numWeights);
			int* wInds = loader.getWIndex(id, numWInds);

			int* tris = loader.triangles(id, numTris, stride, &strideList);

			mdl.numVertices = numTris * 3;
			mdl.verices = new Vector3f[mdl.numVertices];
			if (normals) {
				mdl.hasNormals = true;
				mdl.normals = new Vector3f[mdl.numVertices];
			}
			if (uvs) {

				mdl.uvs = new Vector2f[mdl.numVertices];
			}
			if (colors) {
				mdl.hasNormals = true;
				mdl.vertexColors = new Color4f[mdl.numVertices];
			}

			if (weights && wInds) {
				mdl.hasWeights = true;
				mdl.vertexWeights = new VertexBoneWeight[mdl.numVertices];
				mdl.vertexIndex = new VertexBoneIndex[mdl.numVertices];
			}

			mdl.numFaces = numTris;
			mdl.faces = new Triangle[numTris];

			uint32_t c = 0;

			for (uint32_t t = 0; t < numTris; t++) {

				uint32_t triOffset = t * (stride * 3); // 3 vertices per triangle

				for (uint32_t v = 0; v < 3; v++) {

					uint32_t vOffset = v * stride;

					for (uint32_t s = 0; s < stride; s++) {

						uint32_t index = tris[triOffset + vOffset] + s;

						if (strideList[s] == StrideList::Vertex) {

							uint32_t vIndex = index * 3;
							uint32_t wIndex = index * 4;

							mdl.verices[c].x = vertices[vIndex + 0];
							mdl.verices[c].y = vertices[vIndex + 1];
							mdl.verices[c].z = vertices[vIndex + 2];

							if (mdl.hasWeights) {

								mdl.vertexWeights[c].w1 = weights[wIndex + 0];
								mdl.vertexWeights[c].w2 = weights[wIndex + 1];
								mdl.vertexWeights[c].w3 = weights[wIndex + 2];
								mdl.vertexWeights[c].w4 = weights[wIndex + 3];


								mdl.vertexIndex[c].w1 = wInds[wIndex + 0];
								mdl.vertexIndex[c].w2 = wInds[wIndex + 1];
								mdl.vertexIndex[c].w3 = wInds[wIndex + 2];
								mdl.vertexIndex[c].w4 = wInds[wIndex + 3];
							}


						} else if (strideList[s] == StrideList::Normal) {
							uint32_t vIndex = index * 3;

							mdl.normals[c].x = normals[vIndex + 0];
							mdl.normals[c].y = normals[vIndex + 1];
							mdl.normals[c].z = normals[vIndex + 2];

						} else if (strideList[s] == StrideList::Uv) {
							uint32_t vIndex = index * 2;

							mdl.uvs[c].u = uvs[vIndex + 0];
							mdl.uvs[c].v = uvs[vIndex + 1];

						} else if (strideList[s] == StrideList::Color) {
							uint32_t vIndex = index * 4;

							mdl.vertexColors[c].r = uvs[vIndex + 0];
							mdl.vertexColors[c].g = uvs[vIndex + 1];
							mdl.vertexColors[c].b = uvs[vIndex + 2];
							mdl.vertexColors[c].a = uvs[vIndex + 3];
						}
					}

					if (v == 0) {
						mdl.faces[t].v1 = c;
					} else if (v == 1) {
						mdl.faces[t].v2 = c;
					} else {
						mdl.faces[t].v3 = c;
					}
					c++;
				}
			}

			uint32_t numBones = loader.numBones(id);

			std::function<SkeletonBone(const char*, int)> make = [&](const char* rootName, int index) -> SkeletonBone {

				SkeletonBone bone;

				const char* name = loader.getChildName(id, rootName, index);
				size_t len = strlen(name);

				float* boneTransform = loader.getBoneTransform(id, name);
				int numChilds = loader.numBoneChildren(id, name);

				bone.children = nullptr;
				bone.numChildren = 0;

				bone.boneName = new char[len + 1];
				memcpy((void*)bone.boneName, name, len + 1);
				memcpy(&bone.transform, boneTransform, sizeof(float) * 16);
				bone.boneIndex = loader.getBoneIndex(id, name);

				bone.numChildren = numChilds;
				if (numChilds != 0) {
					bone.children = new SkeletonBone[numChilds];
					for (int i = 0; i < numChilds; i++) {
						bone.children[i] = make(name, i);
					}
				} else {
					bone.children = nullptr;
				}

				delete[] boneTransform;

				return bone;
			};

			if (numBones > 0) {

				SkeletonBone* root = new SkeletonBone();

				const char* rootName = loader.getRootBoneName(id);
				size_t len = strlen(rootName);

				float* boneTransform = loader.getBoneTransform(id, rootName);
				int numChilds = loader.numBoneChildren(id, rootName);

				root->children = nullptr;
				root->numChildren = 0;

				root->boneName = new char[len + 1];
				memcpy((void*)root->boneName, rootName, len + 1);
				memcpy(&root->transform, boneTransform, sizeof(float) * 16);
				root->boneIndex = loader.getBoneIndex(id, rootName);

				root->numChildren = numChilds;
				root->children = new SkeletonBone[numChilds];
				for (int i = 0; i < numChilds; i++) {
					root->children[i] = make(rootName, i);
				}

				mdl.numBones = numBones;
				mdl.skeleton = root;

				delete[] boneTransform;

			}

			delete[] vertices;
			delete[] normals;
			delete[] uvs;
			delete[] colors;

			delete[] tris;
			delete[] strideList;
			delete[] weights;
			delete[] wInds;
		}
		*/
	}

	Model loadModel(const char* fileName) {
		Model m;

		if (endsWithCaseInsensitive(fileName, ".mesh")) {
			createFromMesh(m, fileName);
		} else if (endsWithCaseInsensitive(fileName, ".dae")) {
			createFromDae(m, fileName);
		}

		return m;
	}

}