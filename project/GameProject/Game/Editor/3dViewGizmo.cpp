#include "3dViewGizmo.hpp"


ViewGizmo::ViewGizmo(CEngine* engine) {

	gizmoMesh = (Engine::Graphics::Mesh::CMesh*)engine->getAssetManager()->loadMesh("Data/Meshes/Editor/Gizmo_view_dir.mesh");

}

ViewGizmo::~ViewGizmo() {

}

