#ifndef SETTIGS_HPP
#define SETTIGS_HPP

#include <Config\Config.hpp>
#include "DisplaySettings.hpp"

class Settings
{
public:

	void loadSettings();
	void saveSettings();

	int getWidth() const;
	int getHeigth() const;

	bool getVsync() const;

	float getFoV() const;

	void setWidth(int w);
	void setHeigth(int h);

	void setVsync(bool vsync);

	void setFoV(float fov);

private:

	Config conf;

	DisplaySettings dispSettings;
};

#endif