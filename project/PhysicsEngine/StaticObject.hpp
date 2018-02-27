#ifndef STATIC_OBJECT_HPP
#define STATIC_OBJECT_HPP

/// Internal Includes
#include "Shapes/IPhysShape.hpp"

/// External Includes
#include <glm/glm.hpp>

/// Std Includes

class StaticObject {
public:

//private:

	glm::vec3 position;

	IPhysicsShape* shape;

};

#endif