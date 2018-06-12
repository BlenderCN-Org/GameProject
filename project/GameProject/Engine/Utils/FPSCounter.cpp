
/// Internal Includes
#include "FPSCounter.hpp"

namespace Engine {
	namespace Core {

		FPSCounter::FPSCounter() {
			dtOneSec = 0.0F;
			fpsCounter = 0;
			fps = 0;
		}

		void FPSCounter::update(float dt) {
			dtOneSec += dt;
			fpsCounter++;

			if (dtOneSec > 1.0F) {
				fps = fpsCounter;
				fpsCounter = 0;
				dtOneSec -= 1.0F;
			}
		}

		int FPSCounter::getFPS() const {
			return fps;
		}

	}
}