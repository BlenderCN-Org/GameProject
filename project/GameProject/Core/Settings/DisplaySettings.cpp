#include "DisplaySettings.hpp"
#include <sstream>


DisplaySettings::DisplaySettings() : window(nullptr) {}

void DisplaySettings::setWindow(IWindow * wnd) {
	window = wnd;
}

void DisplaySettings::setRenderEngine(IRenderEngine * re) {
	renderEngine = re;
}

void DisplaySettings::apply() {
	if ( window && renderEngine ) {

		switch ( fullscreenMode ) {
			case FullscreenMode::FULLSCREEN:
			{
				window->setWindowBorderless(true);
				break;
			}
			case FullscreenMode::WINDOWED_FULLSCREEN:
			{
				window->setWindowBorderless(true);
				break;
			}
			case FullscreenMode::WINDOWED:
			{
				window->setWindowBorderless(false);
				break;
			}
			case FullscreenMode::WINDOWED_BORDERLESS:
			{
				window->setWindowBorderless(true);
				break;
			}
			default:
				break;

		}
		window->setWindowSize(screenRes.width, screenRes.height);
	}

}

void DisplaySettings::setResolution(int width, int height) {
	screenRes.width = width;
	screenRes.height = height;
}

void DisplaySettings::setResolution(Resolution res) {
	screenRes = res;
}

Resolution DisplaySettings::getResolution() const {
	return screenRes;
}

void DisplaySettings::getResolution(int & width, int & height) {
	width = screenRes.width;
	height = screenRes.height;
}

void DisplaySettings::setFullscreenMode(FullscreenMode fmode) {
	fullscreenMode = fmode;
}

FullscreenMode DisplaySettings::getFullscreenMode() const {
	return fullscreenMode;
}

void DisplaySettings::setVsyncMode(VSyncMode vsMode) {
	vsync = vsMode;
}

VSyncMode DisplaySettings::getVsyncMode() const {
	return vsync;
}

void DisplaySettings::setFramerateLock(FramerateLock flock) {
	fpsLock = flock;
}

FramerateLock DisplaySettings::getFrameLock() const {
	return fpsLock;
}

Section DisplaySettings::getSection() const {
	Section s;
	s.addString("Resolution", screenRes.toString());
	s.addInteger("Fullscreen", (int)fullscreenMode);
	s.addInteger("Vsync", (int)vsync);
	s.addInteger("Frame limiter", (int)fpsLock);

	return s;
}

std::string Resolution::toString() const {
	std::stringstream ss;
	ss << width << "x" << height;
	return ss.str();
}
