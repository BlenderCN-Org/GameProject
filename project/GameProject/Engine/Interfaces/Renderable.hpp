#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

class Renderable {

public:
	virtual ~Renderable() {};
	virtual void render() = 0;

};

#endif