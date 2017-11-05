#ifndef I_CAMERA_HPP
#define I_CAMERA_HPP

#include <glm/glm.hpp>

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