#include "Vertex.hpp"

Vertex3f::Vertex3f() {
	x = y = z = 0;
}
Vertex3f::Vertex3f(float default) {
	x = y = z = default;
}
Vertex3f::Vertex3f(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vertex2f::Vertex2f() {
	x = y = 0;
}

Vertex2f::Vertex2f(float default) {
	x = y = default;
}

Vertex2f::Vertex2f(float _x, float _y) {
	x = _x;
	y = _y;
}