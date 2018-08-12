
/// Internal Includes
#include "Editor.hpp"

#include "MapObjects/Cell.hpp"

#include "../Engine/Input/Input.hpp"
#include "../Engine/Graphics/Graphics.hpp"
#include "../Engine/Core/Console.hpp"

#include "MapObjects/Sky.hpp"
#include "MapObjects/Map.hpp"
#include "MapObjects/DataTags.hpp"

/// External Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// Std Includes

Editor::Editor(CEngine* engine)
	: pGameEditAccess(nullptr)
	, eState(EditState::NO_MAP) {

	viewGizmo = new ViewGizmo(engine);
	pEngine = engine;

	editorBasicShader = gRenderEngine->createShaderObject();

	std::string vs = Engine::System::readShader("data/shaders/editor/default.vs.glsl");
	std::string fs = Engine::System::readShader("data/shaders/editor/black.fs.glsl");

	editorBasicShader->setShaderCode(ShaderStages::VERTEX_STAGE, vs.c_str());
	editorBasicShader->setShaderCode(ShaderStages::FRAGMENT_STAGE, fs.c_str());

	if (!editorBasicShader->buildShader()) {
		assert(0 && "failed to compile editor shader!\n");
	}

	editorViewProjLoc = editorBasicShader->getShaderUniform("viewProjMatrix");
	editorModelMatLoc = editorBasicShader->getShaderUniform("worldMat");

	cellBorderMesh = gRenderEngine->createMesh();
	cellBorderMesh->init(MeshPrimitiveType::LINE);

	float vertex[8 * 5] = {

		+CELL_SIZE / 2.0F, -200.0F, +CELL_SIZE / 2.0F, 0.0F, 0.0F,
		+CELL_SIZE / 2.0F, +200.0F, +CELL_SIZE / 2.0F, 0.0F, 0.0F,

		+CELL_SIZE / 2.0F, -200.0F, -CELL_SIZE / 2.0F, 0.0F, 0.0F,
		+CELL_SIZE / 2.0F, +200.0F, -CELL_SIZE / 2.0F, 0.0F, 0.0F,

		-CELL_SIZE / 2.0F, -200.0F, +CELL_SIZE / 2.0F, 0.0F, 0.0F,
		-CELL_SIZE / 2.0F, +200.0F, +CELL_SIZE / 2.0F, 0.0F, 0.0F,

		-CELL_SIZE / 2.0F, -200.0F, -CELL_SIZE / 2.0F, 0.0F, 0.0F,
		-CELL_SIZE / 2.0F, +200.0F, -CELL_SIZE / 2.0F, 0.0F, 0.0F,
	};

	cellBorderMesh->setMeshData(vertex, sizeof(vertex), MeshDataLayout::VERT_UV);
	guiInfo.pAssetManager = engine->getAssetManager();

	fileDialog = new FileDialog(guiInfo);
	fileDialog->setAnchorPoint(Engine::Graphics::GuiAnchor::CENTER);
	fileDialog->setSize(600, 600);
	fileDialog->setZIndex(123);
	//fileDialog->setVisible(true);

	skyEdit = new SkyEdit(guiInfo);
	skyEdit->setAnchorPoint(Engine::Graphics::GuiAnchor::TOP_LEFT);
	skyEdit->setSize(200, 200);

	engine->getGui()->addGuiItem(fileDialog);
	engine->getGui()->addGuiItem(skyEdit);
}

Editor::~Editor() {

	pEngine->getGui()->removeGuiItem(fileDialog);
	pEngine->getGui()->removeGuiItem(skyEdit);
	cellBorderMesh->release();
	editorBasicShader->release();

	delete viewGizmo;

	delete objList;
	delete fileDialog;
	delete skyEdit;

	pEngine = nullptr;
}

void Editor::start(Engine::DataLoader::IEditLoader* pEditClass, IMap** ppMap) {

	pGameEditAccess = pEditClass;
	ppActiveMap = ppMap;

	Engine::Core::String assetString = gAssetDataPath;
	assetString += "/Data/";

	fileDialog->showDialog(assetString, true);

}

void Editor::update(float dt) {


	switch (eState) {
	case EditState::NO_MAP:
		updateWaitForMapState(dt);
		break;
	case EditState::LOADING:
		updateLoadingState(dt);
		break;
	case EditState::EDITING:
		updateEditing(dt);
		break;
	case EditState::SAVING:
		updateSaving(dt);
		break;
	case EditState::CLOSING:
		updateClosing(dt);
		break;
	default:
		throw ("Invalid Editor state");
		break;
	}

	//Engine::Input::Input* in = Engine::Input::Input::GetInput();
	//
	//// @ TODO add check for moving around to not tirgger by accedent
	//if (in->releasedThisFrame(EditorKeyBinds::editorSave)) {
	//	std::string path = gAssetDataPath;
	//	path += "Data/Test.map";
	//	pGameEditAccess->save(path.c_str());
	//	printf("Saving changes\n");
	//} else if (in->releasedThisFrame(EditorKeyBinds::editorOpen)) {
	//	Engine::Core::gConsole->print("Loading Map...");
	//	Engine::Core::gConsole->print("E:/GameProjectAssets/Data/GameMap.map");
	//	pGameEditAccess->openFile("E:/GameProjectAssets/Data/GameMap.map");
	//	Engine::Core::gConsole->print("Map Loaded!");
	//}

}

void Editor::render(ShaderSettings& shaderSettings) {

	renderGizmo(shaderSettings);

	renderCellBorders(shaderSettings, (*ppActiveMap)->getCellGrid());

}

void Editor::updateWaitForMapState(float dt) {

	if (SelectStatus::SELECT_STATUS_CONFIRM == fileDialog->getSelectStatus()) {
		eState = EditState::LOADING;
		Engine::Core::String selectedFile = fileDialog->getFileName();
		pGameEditAccess->openFile(selectedFile.cStr());
		fileDialog->setVisible(false);
		skyEdit->setVisible(true);
	}
	if (SelectStatus::SELECT_STATUS_CANCEL == fileDialog->getSelectStatus()) {
		printf("Open Canceled\n");
	}
}

void Editor::updateLoadingState(float dt) {

	if (true == pGameEditAccess->loading()) {

	} else {
		eState = EditState::EDITING;
	}
}

void Editor::updateEditing(float dt) {

	Engine::Input::Input* in = Engine::Input::Input::GetInput();
	if (true == in->releasedThisFrame(EditorKeyBinds::editorSave)) {
		std::string path = gAssetDataPath;
		path += "Data/Test.map";
		pGameEditAccess->save(path.c_str());
		printf("Saving changes\n");
	}

	if (true == in->releasedThisFrame(EditorKeyBinds::editorOpen)) {

		Engine::AssetHandling::EntryData entry;

		SkyData sd;
		sd.cycleTime = 120.0F;
		sd.skyColorDay[0] = 0.8F;
		sd.skyColorDay[1] = 0.0F;
		sd.skyColorDay[2] = 0.48F;

		sd.skyColorNight[0] = 0.024422F;
		sd.skyColorNight[1] = 0.088654F;
		sd.skyColorNight[2] = 0.147314F;

		entry.data = malloc(sizeof(SkyData));
		memcpy(entry.data, &sd, sizeof(SkyData));
		entry.size = sizeof(SkyData);
		entry.tag = SKY_TAG;

		pGameEditAccess->editEntry(3, entry);

		MapData map;
		map.skyId = 3;
		map.cellsX = 9;
		map.cellsY = 9;
		map.cellsPointer = 0;
		
		uint32_t size = sizeof(MapData) + ((map.cellsX * map.cellsY) - 1) * sizeof(uint32_t) ;

		entry.data = malloc(size);

		memcpy(entry.data, &map, sizeof(MapData));
		uint32_t* pp = &((MapData*)entry.data)->cellsPointer;

		for (uint32_t i = 0; i < 9 * 9; i++) {
			pp[i] = 0;
		}

		entry.size = size;
		entry.tag = MAP_TAG;

		pGameEditAccess->editEntry(1, entry);

	}

	(*ppActiveMap)->reloadCheck();

}

void Editor::updateSaving(float dt) {

}

void Editor::updateClosing(float dt) {

}

void Editor::renderCellBorders(ShaderSettings& shaderSettings, Grid<Cell>* cellGrid) {

	editorBasicShader->useShader();
	cellBorderMesh->bind();
	editorBasicShader->bindData(editorViewProjLoc, UniformDataType::UNI_MATRIX4X4, &shaderSettings.viewProjectionMatrix);

	uint32_t numCells = cellGrid->size();

	glm::mat4 objMat;

	for (uint32_t i = 0; i < numCells; i++) {

		objMat = glm::mat4();

		GridItemPosition itemPos = cellGrid->getGridPosition(i);

		objMat = glm::translate(objMat, glm::vec3(itemPos.x * CELL_SIZE, 0, itemPos.y * CELL_SIZE));
		objMat = glm::transpose(objMat);

		editorBasicShader->bindData(editorModelMatLoc, UniformDataType::UNI_MATRIX4X4, &objMat);
		cellBorderMesh->render();
	}

}

void Editor::renderGizmo(ShaderSettings& shaderSettings) {


	glm::mat4 viewRot = shaderSettings.viewMatrix;

	viewRot = glm::inverse(viewRot);
	viewRot[3][0] = 0.0F;
	viewRot[3][1] = 0.0F;
	viewRot[3][2] = 0.0F;

	glm::mat4 objMat = glm::mat4();

	glm::vec3 camPos = shaderSettings.cameraPosition;
	glm::vec3 dir = shaderSettings.cameraDirection;

	viewRot = glm::inverse(viewRot);

	objMat[3][0] = camPos.x + (dir.x * 5);
	objMat[3][1] = camPos.y + (dir.y * 5);
	objMat[3][2] = camPos.z + (dir.z * 5);

	objMat = glm::scale(objMat, glm::vec3(0.3F));

	objMat = glm::transpose(objMat);

	glm::mat4 biasCorrection;
	biasCorrection[3][0] = -0.9F;
	biasCorrection[3][1] = -0.85F;

	glm::mat4 finalTranspose = biasCorrection * shaderSettings.viewProjectionMatrix;

	shaderSettings.activeShader->useShader();
	shaderSettings.activeShader->bindData(shaderSettings.viewProjectionLocation, UniformDataType::UNI_MATRIX4X4, &finalTranspose);
	shaderSettings.activeShader->bindData(shaderSettings.reflectionMatrixLocation, UniformDataType::UNI_MATRIX4X4, &glm::mat4());
	shaderSettings.activeShader->bindData(shaderSettings.clipPlaneLocation, UniformDataType::UNI_FLOAT4, &glm::vec4());
	shaderSettings.activeShader->bindData(shaderSettings.modelMatrixLocation, UniformDataType::UNI_MATRIX4X4, &objMat);

	if (nullptr != viewGizmo->gizmoMesh) {
		viewGizmo->gizmoMesh->bind();
		viewGizmo->gizmoMesh->render();
	}

}