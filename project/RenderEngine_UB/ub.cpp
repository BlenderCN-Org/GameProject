
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <new>

#include <RenderEngine\Utils\ShelfPack.cpp>

#include <RenderEngine\Assets\AnimatedMesh_gl.cpp>
#include <RenderEngine\Assets\Font_gl.cpp>
#include <RenderEngine\Assets\FrameBuffer_gl.cpp>
#include <RenderEngine\Assets\Material_gl.cpp>
#include <RenderEngine\Assets\Mesh_gl.cpp>
#include <RenderEngine\Assets\PixelBuffer_gl.cpp>
#include <RenderEngine\Assets\ShaderObject_gl.cpp>
#include <RenderEngine\Assets\Texture_gl.cpp>

#include <RenderEngine\Assets\Mesh_vk.cpp>

#include <RenderEngine\DataTypes\Colors.cpp>
#include <RenderEngine\DataTypes\Image.cpp>
#include <RenderEngine\DataTypes\Vertex.cpp>

#include <RenderEngine\Window\InputHandling.cpp>
#include <RenderEngine\Window\InputHelper.cpp>
#include <RenderEngine\Window\Window.cpp>
#include <RenderEngine\Window\Win32Window.cpp>
#include <RenderEngine\Window\VKWindow.cpp>
#include <RenderEngine\Window\VulkanWindowHelper.cpp>

#include <RenderEngine\RenderEngine.cpp>
