#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include "GuiItem.hpp"
#include "../Text.hpp"

class GuiLabel : public GuiItem{

public:

	GuiLabel();
	virtual ~GuiLabel();

	//void setPanelMaterial(IMaterial* material);
	void setText(const char* text);

	virtual void render(glm::mat4 &vpMatRef) override;

private:

	const char* text;
	
	Text* textObject;

	int VPMat;
	int TransMat;
	int textColor;
	int textSampler;
};

#endif