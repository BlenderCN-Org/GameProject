#ifndef ICAMERA_HPP
#define ICAMERA_HPP

class ICamera
{
public:

	virtual void init() = 0;
	virtual void release() = 0;

	virtual float* getViewMatrix() = 0;
	virtual float* getPerspectiveMatrix() = 0;
	virtual float* getOrthoMatrix() = 0;

};

#endif