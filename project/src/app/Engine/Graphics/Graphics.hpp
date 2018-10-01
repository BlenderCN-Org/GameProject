#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

class IRenderEngine;
namespace Engine {
	namespace Theme {
		struct GuiTheme;
	}
}

extern IRenderEngine* gRenderEngine;
extern const char* gAssetDataPath;
extern Engine::Theme::GuiTheme* gTheme;

#endif