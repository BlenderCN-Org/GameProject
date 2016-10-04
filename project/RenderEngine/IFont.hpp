#ifndef IFONT_HPP
#define IFONT_HPP

class IFont {

public:

	virtual void init(char* fontName, int size) = 0;
	virtual void release() = 0;

	virtual void setFontSize(int size) = 0;
	virtual void setFont(char* fontName) = 0;

};

#endif