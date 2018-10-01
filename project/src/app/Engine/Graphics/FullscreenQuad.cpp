
/// Internal Includes
#include "FullscreenQuad.hpp"
#include "Graphics.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Graphics {

		FullscreenQuad::FullscreenQuad() {

			mesh = gRenderEngine->createMesh();
			mesh->init(MeshPrimitiveType::TRIANGLE);

			float vertex[6][5] {
				{ 0 - 1, 0 - 1, 0, 0, 0 },
				{ 0 - 1, 0 + 1, 0, 0, 1 },
				{ 0 + 1, 0 + 1, 0, 1, 1 },

				{ 0 - 1, 0 - 1, 0, 0, 0 },
				{ 0 + 1, 0 + 1, 0, 1, 1 },
				{ 0 + 1, 0 - 1, 0, 1, 0 },
			};

			mesh->setMeshData(vertex, sizeof(vertex), MeshDataLayout::VERT_UV);

		}

		FullscreenQuad::~FullscreenQuad() {
			mesh->release();
		}

		void FullscreenQuad::render() {
			mesh->bind();
			mesh->render();
		}
	}
}