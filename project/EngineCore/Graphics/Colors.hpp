#ifndef COLORS_HPP
#define COLORS_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

namespace Engine {
	namespace Colors {

		namespace solid {

			const glm::vec4 WHITE    = glm::vec4(1.0F, 1.0F, 1.0F, 1.0F);
			const glm::vec4 GRAY50   = glm::vec4(0.5F, 0.5F, 0.5F, 1.0F);
			const glm::vec4 BLACK    = glm::vec4(0.0F, 0.0F, 0.0F, 1.0F);
			const glm::vec4 RED      = glm::vec4(1.0F, 0.0F, 0.0F, 1.0F);
			const glm::vec4 GREEN    = glm::vec4(0.0F, 1.0F, 0.0F, 1.0F);
			const glm::vec4 BLUE     = glm::vec4(0.0F, 0.0F, 1.0F, 1.0F);

		}

		namespace transparent75 {

			const glm::vec4 WHITE    = glm::vec4(1.0F, 1.0F, 1.0F, 0.75F);
			const glm::vec4 GRAY50   = glm::vec4(0.5F, 0.5F, 0.5F, 0.75F);
			const glm::vec4 BLACK    = glm::vec4(0.0F, 0.0F, 0.0F, 0.75F);
			const glm::vec4 RED      = glm::vec4(1.0F, 0.0F, 0.0F, 0.75F);
			const glm::vec4 GREEN    = glm::vec4(0.0F, 1.0F, 0.0F, 0.75F);
			const glm::vec4 BLUE     = glm::vec4(0.0F, 0.0F, 1.0F, 0.75F);

		}

		namespace transparent50 {

			const glm::vec4 WHITE    = glm::vec4(1.0F, 1.0F, 1.0F, 0.5F);
			const glm::vec4 GRAY50   = glm::vec4(0.5F, 0.5F, 0.5F, 0.5F);
			const glm::vec4 BLACK    = glm::vec4(0.0F, 0.0F, 0.0F, 0.5F);
			const glm::vec4 RED      = glm::vec4(1.0F, 0.0F, 0.0F, 0.5F);
			const glm::vec4 GREEN    = glm::vec4(0.0F, 1.0F, 0.0F, 0.5F);
			const glm::vec4 BLUE     = glm::vec4(0.0F, 0.0F, 1.0F, 0.5F);

		}

		namespace transparent25 {

			const glm::vec4 WHITE    = glm::vec4(1.0F, 1.0F, 1.0F, 0.25F);
			const glm::vec4 GRAY50   = glm::vec4(0.5F, 0.5F, 0.5F, 0.25F);
			const glm::vec4 BLACK    = glm::vec4(0.0F, 0.0F, 0.0F, 0.25F);
			const glm::vec4 RED      = glm::vec4(1.0F, 0.0F, 0.0F, 0.25F);
			const glm::vec4 GREEN    = glm::vec4(0.0F, 1.0F, 0.0F, 0.25F);
			const glm::vec4 BLUE     = glm::vec4(0.0F, 0.0F, 1.0F, 0.25F);

		}

	}
}


#endif