#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

namespace Engine {
	namespace Core {
		class FPSCounter {
		public:
			FPSCounter();

			void update(float dt);
			int getFPS() const;

		private:
			float dtOneSec;
			int fps;
			int fpsCounter;

		};
	}
}
#endif