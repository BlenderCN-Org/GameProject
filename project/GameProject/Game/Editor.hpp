#ifndef EDITOR_HPP
#define EDITOR_HPP

/// Internal Includes
#include "IMap.hpp"

#include "ShaderSettings.hpp"

#include "Editor/EditorKeyBinds.hpp"
#include "Editor/3dViewGizmo.hpp"

#include "../Engine/Engine.hpp"
#include "../Engine/Graphics/Gui.hpp"
#include "../Engine/Graphics/Gui/GuiWindow.hpp"

#include "Editor/CameraStatusInfo.hpp"
#include "Editor/FileDialog.hpp"
#include "Editor/Toolbox/ObjectList.hpp"
#include "Editor/Toolbox/SkyEdit.hpp"
#include "Editor/ToolSelector.hpp"
#include "Editor/EnvironmentEditor.hpp"

/// External Includes
#include <EngineCore/AssetHandling/Loader/IEditLoader.hpp>

/// Std Includes

enum class EditState {

	NO_MAP,
	LOADING,
	EDITING,
	SAVING,
	CLOSING,
};

class Editor {

public:

	Editor(CEngine* engine);
	~Editor();

	void start(Engine::DataLoader::IEditLoader* pEditClass, IMap** ppMap);

	void update(float dt);

	void render(ShaderSettings& shaderSettings);

private:

	void updateWaitForMapState(float dt);
	void updateLoadingState(float dt);
	void updateEditing(float dt);
	void updateSaving(float dt);
	void updateClosing(float dt);

	void renderCellBorders(ShaderSettings& shaderSettings, Grid<Cell>* cellGrid);
	void renderGizmo(ShaderSettings& shaderSettings);

	EditState eState;

	CEngine* pEngine;
	Engine::Graphics::GuiInfo guiInfo;


	ObjectList* objList;
	FileDialog* fileDialog;
	SkyEdit* skyEdit;
	CameraStatusInfo* cameraStatus;
	ToolSelector* toolSelector;
	EnvironmentEditor* envEditor;

	Engine::Graphics::Gui::StatusBar* statusBar;

	Engine::DataLoader::IEditLoader* pGameEditAccess;
	IMap** ppActiveMap;

	ViewGizmo* viewGizmo;

	IMesh* cellBorderMesh;

	IShaderObject* editorBasicShader;
	int editorViewProjLoc;
	int editorModelMatLoc;

};


#endif