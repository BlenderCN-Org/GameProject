#ifndef STATIC_OBJECT_HPP
#define STATIC_OBJECT_HPP

/// Internal Includes
#include "PhysicsEngine/IPhysShape.hpp"

/// External Includes
#pragma warning( push , 3 )
#include <glm/glm.hpp>
#pragma warning( pop )

/// Std Includes

class StaticObject {
public:

//private:

	glm::vec3 position;

	IPhysicsShape* shape;

};

#endif