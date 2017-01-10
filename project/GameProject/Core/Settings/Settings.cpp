#include "Settings.hpp"

void Settings::loadSettings() {
	Section display = dispSettings.getSection();

	conf.addSection("Display", display);
}

void Settings::saveSettings() {}

int Settings::getWidth() const {
	return 0;
}

int Settings::getHeigth() const {
	return 0;
}

bool Settings::getVsync() const {
	return false;
}

float Settings::getFoV() const {
	return 0.0f;
}

void Settings::setWidth(int w) {}

void Settings::setHeigth(int h) {}

void Settings::setVsync(bool vsync) {}

void Settings::setFoV(float fov) {}