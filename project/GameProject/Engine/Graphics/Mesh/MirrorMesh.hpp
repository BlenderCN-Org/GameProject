#ifndef MIRROR_MESH_HPP
#define MIRROR_MESH_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>
#include <RenderEngine/IMesh.hpp>

/// Std Includes


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

				glm::mat4 reflectionMatrix(glm::vec3 reflectDist = glm::vec3());
				glm::mat4 modelMatrix();

				glm::vec4 getNormal() const;
				glm::vec3 getPos() const;

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
