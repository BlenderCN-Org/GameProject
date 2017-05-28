#include "SceneForm.hpp"
#include "../../Memory/MemoryBuffer.hpp"
#include <Editor_Wrapper\IEditor.hpp>

SceneForm::SceneForm()
{
	memcpy(tag, "SCNE", 4);
}

SceneForm::~SceneForm()
{
}

uint32_t SceneForm::getFormSize()
{
	formSize = 0;

	formSize += sizeof(tag) + sizeof(formID) + sizeof(formSize);
	formSize += sizeof(name) + sizeof(skyColor) + sizeof(fogEnabled) + sizeof(fogColorNear) + sizeof(fogColorFar);
	formSize += sizeof(enableWater);

	return formSize;
}

void SceneForm::getFormData(void * pData)
{
	if (pData == nullptr)
		return;
	uint32_t offset = 0;

	offset = copyToPtr(pData, offset, &tag, sizeof(tag));
	offset = copyToPtr(pData, offset, &formID, sizeof(uint32_t));
	offset = copyToPtr(pData, offset, &formSize, sizeof(uint32_t));
	offset = copyToPtr(pData, offset, &name, sizeof(name));
	offset = copyToPtr(pData, offset, &skyColor, sizeof(skyColor));
	offset = copyToPtr(pData, offset, &fogEnabled, sizeof(fogEnabled));
	offset = copyToPtr(pData, offset, &fogColorNear, sizeof(fogColorNear));
	offset = copyToPtr(pData, offset, &fogColorFar, sizeof(fogColorFar));
	offset = copyToPtr(pData, offset, &enableWater, sizeof(enableWater));

}

BaseForm * SceneForm::createFromData(void * data, uint32_t size)
{
	SceneForm* form = new SceneForm();
	MemoryBuffer memBuff;
	memBuff.setData(data, size);
	memcpy(form->name, memBuff.returnBytes(sizeof(name)), sizeof(name));

	form->skyColor = *(Color*)memBuff.returnBytes(sizeof(Color));
	form->fogEnabled = *(bool*)memBuff.returnBytes(sizeof(bool));
	form->fogColorNear = *(Color*)memBuff.returnBytes(sizeof(Color));
	form->fogColorFar = *(Color*)memBuff.returnBytes(sizeof(Color));
	form->enableWater = *(bool*)memBuff.returnBytes(sizeof(bool));
	/*form->flags = *(uint32_t*)memBuff.returnBytes(sizeof(flags));
	form->nrMenuItems = *(uint32_t*)memBuff.returnBytes(sizeof(flags));

	form->subMenuItems = new SubMenuItem[form->nrMenuItems];

	for (size_t i = 0; i < form->nrMenuItems; i++)
	{
		form->subMenuItems[i].flags = *(uint32_t*)memBuff.returnBytes(sizeof(SubMenuItem::flags));
		memcpy(form->subMenuItems[i].name, memBuff.returnBytes(sizeof(SubMenuItem::name)), sizeof(SubMenuItem::name));
		memcpy(form->subMenuItems[i].command, memBuff.returnBytes(sizeof(SubMenuItem::command)), sizeof(SubMenuItem::command));
	}*/

	memBuff.deleteBuffer();
	return form;
}

uint32_t SceneForm::getType() {
	return OBJECT_TYPE_SCENE;
}

const char * SceneForm::getName() {
	return name;
}
