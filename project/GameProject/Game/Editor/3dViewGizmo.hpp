#ifndef _3D_VIEW_GIZMO_HPP
#define _3D_VIEW_GIZMO_HPP

/// Internal Includes
#include "../../Engine/Graphics/Mesh/Mesh.hpp"
#include "../../Engine/Engine.hpp"

/// External Includes

/// Std Includes

class ViewGizmo {

public:

	ViewGizmo(CEngine* engine);
	virtual ~ViewGizmo();



//private:

	Engine::Graphics::Mesh::CMesh* gizmoMesh;


};



#endif