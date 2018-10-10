#ifndef I_EDITOR_ACCESS_HPP
#define I_EDITOR_ACCESS_HPP

#include <cstdint>

class IEditorAccess {
public:

	// selected scene
	virtual void setActiveScene(uint32_t sceneFormID) = 0;

	// place/remove a object in selected scene
	virtual void placeObject(uint32_t objectFormID) = 0;
	virtual void removeObject(uint32_t objectRefID) = 0;

	// object to select
	virtual void selectObject(uint32_t objectRefID) = 0;
};

#endif