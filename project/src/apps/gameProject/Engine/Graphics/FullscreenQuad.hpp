#ifndef FULLSCREEN_QUAD_HPP
#define FULLSCREEN_QUAD_HPP

/// Internal Includes

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes

namespace Engine {
	namespace Graphics {

		class FullscreenQuad {

		public:
			FullscreenQuad();
			virtual ~FullscreenQuad();

			void render();

		private:

			IMesh * mesh;

		};
	}
}


#endif