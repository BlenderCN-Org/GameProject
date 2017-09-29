#ifndef GUI_ITEM_HPP
#define GUI_ITEM_HPP

#include "GuiAnchorPoint.hpp"

#include <glm/glm.hpp>

class GuiItem {
public:
	GuiItem();
	virtual ~GuiItem();

	void setPosition(int x, int y);
	void setSize(int w, int h);

	void setAnchorPoint(GuiAnchor anchor);

	virtual void render(glm::mat4 &vpMatRef);

protected:

	glm::ivec2 position;
	glm::ivec2 size;

	GuiAnchor anchorPoint;
};

#endif