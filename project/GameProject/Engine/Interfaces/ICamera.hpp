#ifndef I_CAMERA_HPP
#define I_CAMERA_HPP

/// Internal Includes

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

namespace Engine {

	namespace Interfaces {

		class ICamera {
		public:
			virtual ~ICamera() {}

			virtual glm::mat4* getViewMatrix() = 0;
			virtual glm::mat4* getProjectionMatrix() = 0;
		};
	}
}


#endif