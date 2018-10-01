#ifndef EDITOR_ACCESS_HPP
#define EDITOR_ACCESS_HPP

#include "../IEditorAccess.hpp"
#include "Scene.hpp"

class Game;

class EditorAccess :public IEditorAccess {
public:

	EditorAccess();
	virtual ~EditorAccess();

	// IEditorAccess
	virtual void setActiveScene(uint32_t sceneFormID);
	virtual void placeObject(uint32_t objectFormID);
	virtual void removeObject(uint32_t objectRefID);
	virtual void selectObject(uint32_t objectRefID);

	void setGameInstance(Game* g);
	Scene* getCurrentScene();

private:

	Game* game;
	Scene startupScene;
	Scene* selectedScene;
};

#endif