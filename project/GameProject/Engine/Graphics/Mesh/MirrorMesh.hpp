#ifndef MIRROR_MESH_HPP
#define MIRROR_MESH_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/IMesh.hpp>
#include <glm/glm.hpp>

namespace Engine {
	namespace Graphics {
		namespace Mesh {


			class MirrorMesh {
			public:

				MirrorMesh();
				virtual ~MirrorMesh();

				void setSize(glm::vec2 s);
				void setMirrorPosition(glm::vec3 p);
				void setMirrorNormal(glm::vec3 n);

				glm::mat4 reflectionMatrix();
				glm::mat4 modelMatrix();

				void render(bool writeDepth = false);

			private:

				IMesh* mesh;
				glm::vec2 size;
				glm::vec3 pos;
				glm::vec3 normal;

				glm::mat4 rotMat;

			};

		}
	}
}
#endif
