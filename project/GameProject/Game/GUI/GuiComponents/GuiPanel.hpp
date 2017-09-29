#ifndef GUI_PANEL_HPP
#define GUI_PANEL_HPP

#include <RenderEngine/IMaterial.hpp>

#include "GuiItem.hpp"

#include <vector>

class GuiPanel : public GuiItem
{
public:

	GuiPanel();
	virtual ~GuiPanel();

	void addGuiItem(GuiItem item);
	void setPanelMaterial(IMaterial* material);

	virtual void render(glm::mat4 &vpMatRef) override;

private:

	std::vector<GuiItem> subItems;

	IMaterial* panelMaterial;

	int VPMat;
	int TransMat;

};

#endif
