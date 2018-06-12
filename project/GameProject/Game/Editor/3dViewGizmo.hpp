#ifndef _3D_VIEW_GIZMO_HPP
#define _3D_VIEW_GIZMO_HPP

#include "../../Engine/Graphics/Mesh/Mesh.hpp"
#include "../../Engine/Engine.hpp"

class ViewGizmo {

public:

	ViewGizmo(CEngine* engine);
	virtual ~ViewGizmo();



//private:

	Engine::Graphics::Mesh::CMesh* gizmoMesh;


};



#endif