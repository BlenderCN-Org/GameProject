#ifndef GAMEFILEEDITOR_HPP
#define GAMEFILEEDITOR_HPP

#include "GameFile.hpp"

class GameFileEditor : public GameFile {

public:

	virtual void init();
	virtual void load(const char* name);

};

#endif