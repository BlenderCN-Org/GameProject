#ifndef COLOR_HPP
#define COLOR_HPP

struct Color {

	Color() :
		r((float)0xDEADBEEF),
		g((float)0xDEADBEEF),
		b((float)0xDEADBEEF),
		a((float)0xDEADBEEF) {};

	float r, g, b, a;

};

#endif