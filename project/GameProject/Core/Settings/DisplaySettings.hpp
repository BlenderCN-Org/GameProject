#ifndef DISPLAYSETTINGS_HPP
#define DISPLAYSETTINGS_HPP

#include <Config\Config.hpp>

#include <RenderEngine\IRenderEngine.hpp>

struct Resolution
{
	int width;
	int height;

	std::string toString() const;

};

enum class FullscreenMode : unsigned char
{
	FULLSCREEN = 0, //	 full-fullscreen
	WINDOWED_FULLSCREEN = 1, // borderless fullscreen
	WINDOWED = 2, // windowed
	WINDOWED_BORDERLESS = 3, // windowed borderless 
};

enum class VSyncMode : char
{
	VSYNC_ADDAPTIVE = -1,
	VSYNC_OFF = 0,
	VSYNC_ON = 1,
};

enum class FramerateLock : short
{
	FRAME_LOCK_SCREEN = -1,
	FRAME_LOCK_NONE = 0,
	FRAME_LOCK_30 = 30,
	FRAME_LOCK_60 = 60,
};

class DisplaySettings
{

public:

	DisplaySettings();

	void setWindow(IWindow* wnd);
	void setRenderEngine(IRenderEngine* re);

	void apply();
	
	void setResolution(int width, int height);
	void setResolution(Resolution res);

	Resolution getResolution() const;
	void getResolution(int &width, int &height);

	void setFullscreenMode(FullscreenMode fmode);
	FullscreenMode getFullscreenMode() const;

	void setVsyncMode(VSyncMode vsMode);
	VSyncMode getVsyncMode() const;

	void setFramerateLock(FramerateLock flock);
	FramerateLock getFrameLock() const;

	Section getSection() const;

private:
	
	Resolution screenRes;
	FullscreenMode fullscreenMode;
	VSyncMode vsync;
	FramerateLock fpsLock;

	IRenderEngine* renderEngine;

	IWindow* window;

};

#endif