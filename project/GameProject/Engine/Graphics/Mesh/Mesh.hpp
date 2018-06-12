#ifndef STATIC_MESH_HPP
#define STATIC_MESH_HPP

/// Internal Includes
#include "Animation.hpp"
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

				void setAnimationData(Animation* anim);

				IMesh* getIMesh() const;
				void loadMesh(const char* file);

				virtual bool hasAnimations();
				virtual Interfaces::ICAnimation* getAnimData();

				virtual void bind();
				virtual void render();

			private:

				Animation* anim;
				IMesh* mesh;

			};

		}
	}
}

#endif