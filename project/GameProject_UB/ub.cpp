#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <new>

#include <GameProject\Core\CoreGlobals.cpp>

#include <GameProject\Core\System\Console.cpp>
#include <GameProject\Core\System\LibraryLoader.cpp>
#include <GameProject\Core\System\Sys.cpp>
#include <GameProject\Core\System\MemBuffer.cpp>

#include <GameProject\Core\Assets\DataObjects.cpp>
#include <GameProject\Core\Assets\GameFile.cpp>
#include <GameProject\Core\Assets\RenderLayer.cpp>

#include <GameProject\Core\Extensions\SaveExtension.cpp>
#include <GameProject\Core\Extensions\ExtensionHandler.cpp>

#include <GameProject\Core\Input\Input.cpp>

#include <GameProject\Core\AssetManager.cpp>
#include <GameProject\Core\Core.cpp>

#include <GameProject\Game\Input\CameraInput.cpp>

#include <GameProject\Game\Components\BaseComponent.cpp>
#include <GameProject\Game\Components\PhysicsComponent.cpp>
#include <GameProject\Game\Components\RenderComponent.cpp>
#include <GameProject\Game\Components\TransformComponent.cpp>

#include <GameProject/Game/GUI/Gui.cpp>
#include <GameProject/Game/GUI/Text.cpp>

#include <GameProject/Game/GUI/GuiComponents/GuiItem.cpp>
#include <GameProject/Game/GUI/GuiComponents/GuiPanel.cpp>
#include <GameProject/Game/GUI/GuiComponents/GuiLabel.cpp>

#include <GameProject\Game\Camera.cpp>
#include <GameProject\Game\GameObject.cpp>
#include <GameProject\Game\Scene.cpp>
#include <GameProject\Game\EditorAccess.cpp>
#include <GameProject\Game\Menu.cpp>
#include <GameProject\Game\Game.cpp>

#include <GameProject\ExceptionHandling.cpp>
#include <GameProject\main.cpp>