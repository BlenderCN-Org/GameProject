#ifndef COLORS_HPP
#define COLORS_HPP

struct HSV
{
	float h;
	float s;
	float v;
	HSV(float _h, float _s, float _v);
	HSV() {};
};

struct RGB
{
	float r;
	float g;
	float b;
	RGB(float _r, float _g, float _b);
	RGB() {};
};

RGB HSV2RGB(HSV hsv);
HSV RGB2HSV(RGB rgb);

#endif