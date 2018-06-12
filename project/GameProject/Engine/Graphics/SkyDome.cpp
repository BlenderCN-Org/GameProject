
/// Internal Includes
#include "SkyDome.hpp"
#include "Graphics.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/// Internal Includes

/// External Includes

/// Std Includes
#include <vector>

struct Vertex {
	float x, y, z;
	float u, v;
};

SkyDome::SkyDome() : skydome(nullptr) {
	skydome = gRenderEngine->createMesh();
	skydome->init(MeshPrimitiveType::TRIANGLE);

	float radius = 2.0F;

	std::vector<std::vector<Vertex>> verts;
	// Iterate through phi, theta then convert r,theta,phi to  XYZ
	for (float theta = 0.0F; theta < glm::pi<float>(); theta += glm::pi<float>() / 30.0F) // Elevation [0, PI]
	{
		std::vector<Vertex> lineVerts;
		for (float phi = 0.0F; phi < 2.0F * glm::pi<float>(); phi += glm::pi<float>() / 10.0F) // Azimuth [0, 2PI]
		{
			Vertex point;
			point.x = radius * sin(phi) * sin(theta);
			point.y = radius            * cos(theta);
			point.z = radius * cos(phi) * sin(theta);

			lineVerts.push_back(point);
		}

		verts.push_back(lineVerts);
	}

	
	size_t elevateCount = verts.size();
	size_t ringCount = verts[0].size();

	std::vector<Vertex> v;

	for (size_t r = 0; r < ringCount; r++) {

		for (size_t e = 0; e < elevateCount - 1; e++) {

			Vertex p1 = verts[e][r];
			Vertex p2 = verts[e][(r + 1) % ringCount];

			Vertex p3 = verts[(e + 1) % elevateCount][r];
			Vertex p4 = verts[(e + 1) % elevateCount][(r + 1) % ringCount];

			v.push_back(p1);
			v.push_back(p2);
			v.push_back(p3);
			
			v.push_back(p3);
			v.push_back(p2);
			v.push_back(p4);

		}
	}

	skydome->setMeshData(v.data(), v.size() * sizeof(Vertex), MeshDataLayout::VERT_UV);

}

SkyDome::~SkyDome() {
	if (skydome) {
		skydome->release();
	}
}

void SkyDome::render() {
	skydome->bind();
	skydome->render();
}