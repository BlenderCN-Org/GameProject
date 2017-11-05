#ifndef CAMERA_HPP
#define CAMERA_HPP

/// Internal Includes
#include "../Interfaces/ICamera.hpp"

namespace Engine {
	namespace Core {
		class Camera : public Interfaces::ICamera {

		public:
			virtual ~Camera() {}


			virtual glm::mat4* getViewMatrix();
			virtual glm::mat4* getProjectionMatrix();

		private:
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};

	}
}

#endif
