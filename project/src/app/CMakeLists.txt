
set (SRC

	main.cpp
	debug.cpp
	debug.hpp
	ExceptionHandling.cpp
	ExceptionHandling.hpp
	FileStructs.hpp
	IAssetManager.hpp
	IEditorAccess.hpp
	IExtension.hpp
	
	Engine/Engine.hpp
	Engine/Engine.cpp
	Engine/TemporaryStorage.hpp
	Engine/ThreadManager.hpp
	Engine/ThreadManager.cpp
	
	Engine/Core/Allocator.hpp
	Engine/Core/Camera.hpp
	Engine/Core/Camera.cpp
	Engine/Core/Console.hpp
	Engine/Core/Console.cpp
	Engine/Core/Grid.hpp
	Engine/Core/Settings.hpp
	Engine/Core/Settings.cpp
	
	Engine/Graphics/FullscreenQuad.cpp
	Engine/Graphics/FullscreenQuad.hpp
	Engine/Graphics/Graphics.cpp
	Engine/Graphics/Graphics.hpp
	Engine/Graphics/Gui.cpp
	Engine/Graphics/Gui.hpp
	Engine/Graphics/GuiInputMapping.hpp
	Engine/Graphics/GuiItem.cpp
	Engine/Graphics/GuiItem.hpp
	Engine/Graphics/GuiShaders.hpp
	Engine/Graphics/GuiTheme.hpp
	Engine/Graphics/ShaderContainer.hpp
	Engine/Graphics/SkyDome.cpp
	Engine/Graphics/SkyDome.hpp
	
	Engine/Graphics/GraphicsResources/arrow_left.png.h
	Engine/Graphics/GraphicsResources/arrow_right.png.h
	
	Engine/Graphics/Gui/Button.cpp
	Engine/Graphics/Gui/Button.hpp
	Engine/Graphics/Gui/ComboBox.cpp
	Engine/Graphics/Gui/ComboBox.hpp
	Engine/Graphics/Gui/Cursor.cpp
	Engine/Graphics/Gui/Cursor.hpp
	Engine/Graphics/Gui/GuiConstants.hpp
	Engine/Graphics/Gui/GuiWindow.cpp
	Engine/Graphics/Gui/GuiWindow.hpp
	Engine/Graphics/Gui/Label.cpp
	Engine/Graphics/Gui/Label.hpp
	Engine/Graphics/Gui/List.cpp
	Engine/Graphics/Gui/List.hpp
	Engine/Graphics/Gui/Panel.cpp
	Engine/Graphics/Gui/Panel.hpp
	Engine/Graphics/Gui/ProgressBar.cpp
	Engine/Graphics/Gui/ProgressBar.hpp
	Engine/Graphics/Gui/ScrollArea.cpp
	Engine/Graphics/Gui/ScrollArea.hpp
	Engine/Graphics/Gui/ScrollBar.cpp
	Engine/Graphics/Gui/ScrollBar.hpp
	Engine/Graphics/Gui/StatusBar.cpp
	Engine/Graphics/Gui/StatusBar.hpp
	Engine/Graphics/Gui/TabView.cpp
	Engine/Graphics/Gui/TabView.hpp
	Engine/Graphics/Gui/Text.cpp
	Engine/Graphics/Gui/Text.hpp
	Engine/Graphics/Gui/TextArea.cpp
	Engine/Graphics/Gui/TextArea.hpp
	Engine/Graphics/Gui/TextureView.cpp
	Engine/Graphics/Gui/TextureView.hpp
	Engine/Graphics/Gui/Tree.cpp
	Engine/Graphics/Gui/Tree.hpp
	
	Engine/Graphics/Mesh/Animation.cpp
	Engine/Graphics/Mesh/Animation.hpp
	Engine/Graphics/Mesh/Mesh.cpp
	Engine/Graphics/Mesh/Mesh.hpp
	Engine/Graphics/Mesh/MirrorMesh.cpp
	Engine/Graphics/Mesh/MirrorMesh.hpp
	
	Engine/Graphics/Textures/Texture2D.cpp
	Engine/Graphics/Textures/Texture2D.hpp
	Engine/Graphics/Textures/Texture2DReference.cpp
	Engine/Graphics/Textures/Texture2DReference.hpp

	Engine/Input/Input.cpp
	Engine/Input/Input.hpp
	Engine/Input/ScanCodes.hpp
	
	Engine/Utils/FPSCounter.cpp
	Engine/Utils/FPSCounter.hpp
	Engine/Utils/MemoryBuffer.cpp
	Engine/Utils/MemoryBuffer.hpp
	
	Game/Camera.cpp
	Game/Camera.hpp
	Game/CameraInput.cpp
	Game/CameraInput.hpp
	Game/CameraPath.cpp
	Game/CameraPath.hpp
	Game/Editor.cpp
	Game/Editor.hpp
	Game/Game.cpp
	Game/Game.hpp
	Game/GameObject.cpp
	Game/GameObject.hpp
	Game/IMap.hpp
	Game/IPlayer.hpp
	Game/MainMenu.cpp
	Game/MainMenu.hpp
	Game/PhysicsParticle.cpp
	Game/PhysicsParticle.hpp
	Game/Player.cpp
	Game/Player.hpp
	Game/RenderBatch.cpp
	Game/RenderBatch.hpp
	Game/ShaderSettings.hpp
	
	Game/Asteroids/Asteroid.cpp
	Game/Asteroids/Asteroid.hpp
	Game/Asteroids/AsteroidsMap.cpp
	Game/Asteroids/AsteroidsMap.hpp
	Game/Asteroids/Bullet.cpp
	Game/Asteroids/Bullet.hpp
	Game/Asteroids/Ship.cpp
	Game/Asteroids/Ship.hpp

	Game/Editor/3dViewGizmo.cpp
	Game/Editor/3dViewGizmo.hpp
	Game/Editor/CameraStatusInfo.cpp
	Game/Editor/CameraStatusInfo.hpp
	Game/Editor/EditorKeyBinds.hpp
	Game/Editor/EnvironmentEditor.cpp
	Game/Editor/EnvironmentEditor.hpp
	Game/Editor/FileDialog.cpp
	Game/Editor/FileDialog.hpp
	Game/Editor/ToolSelector.cpp
	Game/Editor/ToolSelector.hpp
	
	Game/Editor/Resources/file_x16.png.h
	Game/Editor/Resources/folder_x16.png.h
	Game/Editor/Resources/SelectorGradient.png.h
	
	Game/Editor/Toolbox/MapHierarchy.cpp
	Game/Editor/Toolbox/MapHierarchy.hpp
	Game/Editor/Toolbox/ObjectList.cpp
	Game/Editor/Toolbox/ObjectList.hpp
	Game/Editor/Toolbox/SkyEdit.cpp
	Game/Editor/Toolbox/SkyEdit.hpp
	
	Game/MapObjects/Cell.cpp
	Game/MapObjects/Cell.hpp
	Game/MapObjects/DataTags.hpp
	Game/MapObjects/Map.cpp
	Game/MapObjects/Map.hpp
	Game/MapObjects/Scene.cpp
	Game/MapObjects/Scene.hpp
	Game/MapObjects/Sky.cpp
	Game/MapObjects/Sky.hpp
	Game/MapObjects/Terrain.cpp
	Game/MapObjects/Terrain.hpp
)

add_executable(prog ${SRC})
target_link_libraries(prog PRIVATE config_lib core_lib physicsEngine_lib assetLib_lib)
target_include_directories(prog PRIVATE ${RENDER_ENGINE_IF})
target_include_directories(prog PRIVATE ${GLM_INCLUDE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})
set_target_properties(prog PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

if(MAKE_SERVER)
target_compile_definitions(prog PRIVATE OPT_SERVER=1)
else()
target_compile_definitions(prog PRIVATE OPT_SERVER=0)
endif()
if(MAKE_CLIENT)
message("Client on")
target_compile_definitions(prog PRIVATE OPT_CLIENT=1)
else()
message("Client off")
target_compile_definitions(prog PRIVATE OPT_CLIENT=0)
endif()

