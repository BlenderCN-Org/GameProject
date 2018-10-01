
/// Internal Includes
#include "Camera.hpp"

/// External Includes

/// Std Includes

namespace Engine {
	namespace Core {

		glm::mat4* Camera::getViewMatrix() {
			return &projectionMatrix;
		}

		glm::mat4* Camera::getProjectionMatrix() {
			return &projectionMatrix;
		}

	}
}