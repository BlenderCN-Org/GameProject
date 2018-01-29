#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "../Interfaces/ISettings.hpp"

/// External Includes
#include <Config/Config.hpp>


namespace Engine {
	namespace Core {

		class Settings : public Interfaces::ISettings {

		public:

			Settings();

			virtual void setVSync(bool vsync);
			virtual void setResolution(Interfaces::Resolution res);
			virtual void setFullscreenMode(Interfaces::FullscreenMode fmode);

			virtual bool vSync() const;
			virtual Interfaces::Resolution resolution() const;
			virtual Interfaces::FullscreenMode fullscreenMode() const;

		private:

			Config confFile;

		};
	}
}

#endif