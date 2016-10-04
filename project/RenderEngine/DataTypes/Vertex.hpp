#ifndef VERTEX_HPP
#define VERTEX_HPP

struct Vertex3f
{
	float x;
	float y;
	float z;

	Vertex3f();
	Vertex3f(float default);
	Vertex3f(float x, float y, float z);

};

struct Vertex2f
{
	float x;
	float y;

	Vertex2f();
	Vertex2f(float default);
	Vertex2f(float x, float y);
};

#endif