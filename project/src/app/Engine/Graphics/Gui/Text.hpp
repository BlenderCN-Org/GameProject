#ifndef TEXT_HPP
#define TEXT_HPP

/// Internal Includes

/// External Includes
#include <AssetHandling/IAssetManager.hpp>
#include <Core/FormattedString.hpp>

#include <RenderEngine/IRenderEngine.hpp>

class Text {

public:
	Text(Engine::AssetHandling::IAssetManager* assetMan);
	virtual ~Text();

	int calcTextWidth(const Engine::Core::FormattedString& text);
	void setText(const Engine::Core::FormattedString& text);

	int getTextWidth() const;
	int getTextHeight() const;
	int getFontSize() const;

	void render(int textureSlot);

private:

	IReFont* font;
	IMesh* textMesh;

	int width;
	int height;

};

#endif