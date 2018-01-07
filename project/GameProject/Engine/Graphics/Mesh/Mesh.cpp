
/// Internal Includes
#include "../Graphics.hpp"
#include "Mesh.hpp"
#include "../../Utils/MemoryBuffer.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>
#include <AssetLib/AssetLib.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>


/// Std Includes
#include <vector>

namespace Engine {
	namespace Graphics {
		namespace Mesh {

			// @temporary

			struct Header {
				char tag[4];
				uint16_t major;
				uint16_t minor;
			};

			struct BoolFlags {
				uint32_t meshCount;
				bool useVNormals;
				bool useVColors;
				bool useVUV;
				bool padding;
				uint32_t vertCount;
				uint32_t triangleCount;
			};

			struct Triangle {
				uint32_t v1;
				uint32_t v2;
				uint32_t v3;
			};

			struct Vertex5 {
				glm::vec3 pos;
				glm::vec2 uv;
				glm::vec4 color;
			};

			struct Bone {
				int32_t id;
				int32_t parent;
				glm::vec3 headPos;
				glm::vec3 tailPos;
			};

			void makeCube(std::vector<Vertex5> &verts, glm::vec3 pos) {
				glm::vec3 vo1(-0.01f, -0.01f, 0.01f);
				glm::vec3 vo2(-0.01f, 0.01f, 0.01f);
				glm::vec3 vo3(-0.01f, -0.01f, -0.01f);
				glm::vec3 vo4(-0.01f, 0.01f, -0.01f);
				glm::vec3 vo5(0.01f, -0.01f, 0.01f);
				glm::vec3 vo6(0.01f, 0.01f, 0.01f);
				glm::vec3 vo7(0.01f, -0.01f, -0.01f);
				glm::vec3 vo8(0.01f, 0.01f, -0.01f);

				glm::vec3 p = pos;

				Vertex5 v1 = { p + vo1, glm::vec2(0.0f, 0.0f) };
				Vertex5 v2 = { p + vo2, glm::vec2(0.0f, 0.0f) };
				Vertex5 v3 = { p + vo3, glm::vec2(0.0f, 0.0f) };
				Vertex5 v4 = { p + vo4, glm::vec2(0.0f, 0.0f) };
				Vertex5 v5 = { p + vo5, glm::vec2(0.0f, 0.0f) };
				Vertex5 v6 = { p + vo6, glm::vec2(0.0f, 0.0f) };
				Vertex5 v7 = { p + vo7, glm::vec2(0.0f, 0.0f) };
				Vertex5 v8 = { p + vo8, glm::vec2(0.0f, 0.0f) };

				verts.push_back(v2); verts.push_back(v3); verts.push_back(v1);
				verts.push_back(v4); verts.push_back(v7); verts.push_back(v3);

				verts.push_back(v8); verts.push_back(v5); verts.push_back(v7);
				verts.push_back(v6); verts.push_back(v1); verts.push_back(v5);

				verts.push_back(v7); verts.push_back(v1); verts.push_back(v3);
				verts.push_back(v4); verts.push_back(v6); verts.push_back(v8);

				verts.push_back(v2); verts.push_back(v4); verts.push_back(v3);
				verts.push_back(v4); verts.push_back(v8); verts.push_back(v7);

				verts.push_back(v8); verts.push_back(v6); verts.push_back(v5);
				verts.push_back(v6); verts.push_back(v2); verts.push_back(v1);

				verts.push_back(v7); verts.push_back(v5); verts.push_back(v1);
				verts.push_back(v4); verts.push_back(v2); verts.push_back(v6);
			}

			void makeLine(std::vector<Vertex5> &verts, glm::vec3 head, glm::vec3 tail) {
				glm::vec3 vo1(-0.005f, -0.005f, 0.005f);
				glm::vec3 vo2(-0.005f, 0.005f, 0.005f);
				glm::vec3 vo3(-0.005f, -0.005f, -0.005f);
				glm::vec3 vo4(-0.005f, 0.005f, -0.005f);
				glm::vec3 vo5(0.005f, -0.005f, 0.005f);
				glm::vec3 vo6(0.005f, 0.005f, 0.005f);
				glm::vec3 vo7(0.005f, -0.005f, -0.005f);
				glm::vec3 vo8(0.005f, 0.005f, -0.005f);

				glm::vec3 p = head;
				glm::vec3 p2 = tail;

				glm::vec3 ori(0);
				glm::vec3 dir = glm::vec3(1, 0, 0) * glm::distance(p, p2);

				Vertex5 v1 = { ori + vo1, glm::vec2(0.0f, 0.0f) };
				Vertex5 v2 = { ori + vo2, glm::vec2(0.0f, 0.0f) };
				Vertex5 v3 = { ori + vo3, glm::vec2(0.0f, 0.0f) };
				Vertex5 v4 = { ori + vo4, glm::vec2(0.0f, 0.0f) };
				Vertex5 v5 = { dir + vo5, glm::vec2(0.0f, 0.0f) };
				Vertex5 v6 = { dir + vo6, glm::vec2(0.0f, 0.0f) };
				Vertex5 v7 = { dir + vo7, glm::vec2(0.0f, 0.0f) };
				Vertex5 v8 = { dir + vo8, glm::vec2(0.0f, 0.0f) };

				// rotate vertics

				glm::vec3 a = glm::normalize(p2 - p);
				glm::vec3 b = glm::vec3(1, 0, 0);

				float angle = glm::angle(a, b);

				glm::vec3 normal = -glm::normalize(cross(p2 - p, b));

				v1.pos = glm::rotate(v1.pos, angle, normal);
				v2.pos = glm::rotate(v2.pos, angle, normal);
				v3.pos = glm::rotate(v3.pos, angle, normal);
				v4.pos = glm::rotate(v4.pos, angle, normal);
				v5.pos = glm::rotate(v5.pos, angle, normal);
				v6.pos = glm::rotate(v6.pos, angle, normal);
				v7.pos = glm::rotate(v7.pos, angle, normal);
				v8.pos = glm::rotate(v8.pos, angle, normal);

				// translate vertices
				v1.pos += p;
				v2.pos += p;
				v3.pos += p;
				v4.pos += p;
				v5.pos += p;
				v6.pos += p;
				v7.pos += p;
				v8.pos += p;

				verts.push_back(v2); verts.push_back(v3); verts.push_back(v1);
				verts.push_back(v4); verts.push_back(v7); verts.push_back(v3);

				verts.push_back(v8); verts.push_back(v5); verts.push_back(v7);
				verts.push_back(v6); verts.push_back(v1); verts.push_back(v5);

				verts.push_back(v7); verts.push_back(v1); verts.push_back(v3);
				verts.push_back(v4); verts.push_back(v6); verts.push_back(v8);

				verts.push_back(v2); verts.push_back(v4); verts.push_back(v3);
				verts.push_back(v4); verts.push_back(v8); verts.push_back(v7);

				verts.push_back(v8); verts.push_back(v6); verts.push_back(v5);
				verts.push_back(v6); verts.push_back(v2); verts.push_back(v1);

				verts.push_back(v7); verts.push_back(v5); verts.push_back(v1);
				verts.push_back(v4); verts.push_back(v2); verts.push_back(v6);
			}

			void* createVertUVData(void* meshData, uint32_t &size) {

				Engine::Core::MemoryBuffer memBuff;
				memBuff.setData(meshData, size);

				Header* h = (Header*)memBuff.returnBytes<Header>(sizeof(Header));

				int* tag = (int*)h->tag;

				int* meshTag = (int*)"MESH";
				int* skelTag = (int*)"SKEL";

				if (*tag != *meshTag && *tag != *skelTag) {
					//gConsole->print("Invalid mesh file\n");
					size = 0;
					return nullptr;
				}

				//gConsole->print("Mesh version: %d.%d\n", h->major, h->minor);
				if (h->major == 1 && h->minor == 0) {
					BoolFlags* bf = (BoolFlags*)memBuff.returnBytes<BoolFlags>(sizeof(BoolFlags));

					glm::vec3* vertices = (glm::vec3*)memBuff.returnBytes<glm::vec3>(sizeof(glm::vec3) * bf->vertCount);
					glm::vec3* normals = bf->useVNormals ? (glm::vec3*)memBuff.returnBytes<glm::vec3>(sizeof(glm::vec3) * bf->vertCount) : nullptr;
					glm::vec4* colors = bf->useVColors ? (glm::vec4*)memBuff.returnBytes<glm::vec4>(sizeof(glm::vec4) * bf->vertCount) : nullptr;
					glm::vec2* uv = bf->useVUV ? (glm::vec2*)memBuff.returnBytes<glm::vec2>(sizeof(glm::vec2) * bf->vertCount) : nullptr;

					Triangle* triangles = (Triangle*)memBuff.returnBytes<Triangle>(sizeof(Triangle) * bf->triangleCount);

					std::vector<Vertex5> verts;

					for (size_t i = 0; i < bf->triangleCount; i++) {
						Triangle t = triangles[i];

						glm::vec4 c1(1), c2(1), c3(1);

						if (bf->useVColors) {
							c1 = glm::vec4(colors[t.v1]);
							c2 = glm::vec4(colors[t.v2]);
							c3 = glm::vec4(colors[t.v3]);
						}

						Vertex5 v1 = { vertices[t.v1], glm::vec2(0.0f, 0.0f), c1 };
						Vertex5 v2 = { vertices[t.v2], glm::vec2(0.0f, 0.0f), c2 };
						Vertex5 v3 = { vertices[t.v3], glm::vec2(0.0f, 0.0f), c3 };

						verts.push_back(v1);
						verts.push_back(v2);
						verts.push_back(v3);
					}

					int triCount = (int)bf->triangleCount;

					memBuff.deleteBuffer();

					size = (uint32_t)verts.size() * sizeof(Vertex5);

					Vertex5* v = new Vertex5[verts.size()];
					memcpy(v, verts.data(), size);

					//gConsole->print("NrVerts: %d\nNrTris: %d\n", (int)verts.size(), triCount);

					return v;
				} else if (h->major == 1 && h->minor == 1) {
					uint32_t* nrObj = (uint32_t*)memBuff.returnBytes<uint32_t>(sizeof(uint32_t));
					uint32_t* nrBones = (uint32_t*)memBuff.returnBytes<uint32_t>(sizeof(uint32_t));
					//gConsole->print("nrBones: %d\n", (int)*nrBones);

					Bone* bones = (Bone*)memBuff.returnBytes<Bone>(sizeof(Bone) * (*nrBones));

					std::vector<Vertex5> verts;

					for (size_t i = 0; i < (*nrBones); i++) {
						bones[i].headPos *= 1.0f;
						bones[i].tailPos *= 1.0f;

						//printf("b (%f, %f, %f)\n", bones[i].headPos.x, bones[i].headPos.y, bones[i].headPos.z);

						//makeCube(verts, bones[i].headPos);
						//makeCube(verts, bones[i].tailPos);

						if (bones[i].id == 0 || bones[i].parent == 0) {
							makeCube(verts, bones[i].headPos);
						}
						makeCube(verts, bones[i].tailPos);
						makeLine(verts, bones[i].headPos, bones[i].tailPos);
					}

					memBuff.deleteBuffer();
					size = (uint32_t)verts.size() * sizeof(Vertex5);

					Vertex5* v = new Vertex5[verts.size()];
					memcpy(v, verts.data(), size);

					return v;
				}
				return nullptr;
			}

			// @temporary end






			CMesh::CMesh() : anim(nullptr), mesh(nullptr) {
				mesh = gRenderEngine->createMesh();
				mesh->init(MeshPrimitiveType::TRIANGLE);
			}

			CMesh::~CMesh() {
				if (mesh) {
					mesh->release();
				}
				if (anim) {
					delete anim;
				}
			}

			void CMesh::setAnimationData(Animation* _anim) {
				anim = _anim;
			}


			IMesh* CMesh::getIMesh() const {
				return mesh;
			}

			void CMesh::loadMesh(const char* file) {
				uint32_t dataSize = 0;

				std::string path = "";

				if (gAssetDataPath != nullptr) {
					path = gAssetDataPath;
				}

				path += file;

				void* data = AssetLib::fileToMemory(path.c_str(), dataSize);
				if (data) {
					void* vertData = createVertUVData(data, dataSize);
					delete data;

					mesh->setMeshData(vertData, dataSize, MeshDataLayout::VERT_UV_COL);
					delete vertData;
				}
			}

			bool CMesh::hasAnimations() {
				bool hasAnim = false;
				if (anim) {
					hasAnim = true;
				}
				return hasAnim;
			}

			Interfaces::ICAnimation* CMesh::getAnimData() {
				return anim;
			}

			void CMesh::bind() {
				mesh->bind();
			}

			void CMesh::render() {
				mesh->render();
			}

		}
	}
}
