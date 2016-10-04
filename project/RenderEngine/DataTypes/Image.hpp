#ifndef IMAGE_HPP
#define IMAGE_HPP

class Image {

public:

	void init(int x, int y);
	void release();

private:

	void* pixelData;

};

#endif