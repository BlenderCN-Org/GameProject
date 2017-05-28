#ifndef SCENEFORM_HPP
#define SCENEFORM_HPP

#include "BaseForm.hpp"
#include "../Structs/Color.hpp"

class SceneForm : public BaseForm
{
	friend class AssetManager;
	friend class QueryDataExtension;
	friend class EditDataExtension;

public:
	SceneForm();
	~SceneForm();

	const char* getName();

private:

	// Inherited via BaseForm
	virtual uint32_t getFormSize() override;
	virtual void getFormData(void * pData) override;
	virtual BaseForm* createFromData(void * data, uint32_t size) override;

	virtual uint32_t getType() override;

	char name[128];
	Color skyColor;
	bool fogEnabled;
	Color fogColorNear;
	Color fogColorFar;
	bool enableWater;
};

#endif