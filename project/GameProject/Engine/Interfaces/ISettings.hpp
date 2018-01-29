#ifndef I_SETTINGS_HPP
#define I_SETTINGS_HPP

namespace Engine {

	namespace Interfaces {

		struct Resolution {
			int width;
			int height;
		};

		enum class FullscreenMode {
			FULLSCREEN,
			BORDERLESS_FS,
			WINDOWED,
			BORDERLESS_W
		};

		class ISettings {

		public:
			virtual ~ISettings() {};

			virtual void setVSync(bool vsync) = 0;
			virtual void setResolution(Resolution res) = 0;
			virtual void setFullscreenMode(FullscreenMode fmode) = 0;

			virtual bool vSync() const = 0;
			virtual Resolution resolution() const = 0;
			virtual FullscreenMode fullscreenMode() const = 0;

		};
	}
}

#endif