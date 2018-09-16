#ifndef TOOL_SELECTOR_HPP
#define TOOL_SELECTOR_HPP

/// Internal Includes
#include "../../Engine/Graphics/GuiItem.hpp"
#include "../../Engine/Graphics/ShaderContainer.hpp"

#include "../../Engine/Graphics/GuiItem.hpp"

/// External Includes
#include <RenderEngine/IRenderEngine.hpp>

/// Std Includes
#include <vector>

class ToolInfo {

};

class ToolSelector : public Engine::Graphics::GuiItem {

public:

	ToolSelector(Engine::Graphics::GuiInfo& info);
	virtual ~ToolSelector();

	virtual bool isFocusable() const override;
	virtual bool hasFocusableItems() const override;

	virtual void update(float dt, Engine::Graphics::GuiHitInfo& hitInfo, GuiItem* currentFocus) override;
	virtual void render(glm::mat4 &vpMatRef, Engine::Graphics::GuiShaderContainer& shaderContainer) override;

	void clearTools();
	void addTool(ToolInfo& tool);
	
private:

	std::vector<ToolInfo> selectableTools;

	Engine::Graphics::Texture::Texture2D* backgroundTexture;
	IMesh* radialMesh;
};


#endif