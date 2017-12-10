#ifndef STATIC_MESH_HPP
#define STATIC_MESH_HPP

/// Internal Includes

#include "../../Interfaces/ICMesh.hpp"

/// External Includes
#include <RenderEngine/IMesh.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {
		namespace Mesh {

			class CMesh : public Interfaces::ICMesh {
			
			public:
				CMesh();
				virtual ~CMesh();

				IMesh* getIMesh() const;
				void loadMesh(const char* file);

				virtual bool hasAnimations();

				virtual void bind();
				virtual void render();

			private:

				IMesh* mesh;

			};

		}
	}
}

#endif