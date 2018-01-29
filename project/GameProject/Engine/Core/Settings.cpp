#include "Settings.hpp"

namespace Engine {
	namespace Core {

		Settings::Settings() {
			confFile.read("Data/Engine.cfg");
		}

		void Settings::setVSync(bool vsync) {
			confFile.addBoolean("", "vsync", vsync);
			confFile.write("Data/Engine.cfg");
		}

		void Settings::setResolution(Interfaces::Resolution res) {
			confFile.addInteger("", "width", res.width);
			confFile.addInteger("", "height", res.height);
			confFile.write("Data/Engine.cfg");
		}

		void Settings::setFullscreenMode(Interfaces::FullscreenMode fmode) {
			int mode = (int)fmode;
			confFile.addInteger("", "fullscreenMode", mode);
			confFile.write("Data/Engine.cfg");
		}

		bool Settings::vSync() const {
			return confFile.getBoolean("", "vsync", false);
		}

		Interfaces::Resolution Settings::resolution() const {
			Interfaces::Resolution res;

			res.width = confFile.getInteger("", "width", 1280);
			res.height = confFile.getInteger("", "height", 720);

			return res;
		}

		Interfaces::FullscreenMode Settings::fullscreenMode() const {

			Interfaces::FullscreenMode mode;

			mode = (Interfaces::FullscreenMode)confFile.getInteger("", "fullscreenMode", (int)Interfaces::FullscreenMode::WINDOWED);

			return mode;
		}
	}
}