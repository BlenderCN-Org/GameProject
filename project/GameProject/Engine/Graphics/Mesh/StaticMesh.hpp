#ifndef STATIC_MESH_HPP
#define STATIC_MESH_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/IMesh.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Mesh {

			class StaticMesh {
			
			public:
				StaticMesh();
				virtual ~StaticMesh();

				void loadMesh(const char* file);

				void bind();
				void render();

			private:

				IMesh* mesh;

			};

		}
	}
}

#endif