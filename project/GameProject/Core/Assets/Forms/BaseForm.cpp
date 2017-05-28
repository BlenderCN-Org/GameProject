#include "BaseForm.hpp"

#include "MenuForm.hpp"
#include "SceneForm.hpp"
#include <intrin.h>

#include <memory>

std::map<uint32_t, BaseForm*> holder = std::map<uint32_t, BaseForm*>();

uint32_t copyToPtr(void* ptr, uint32_t offset, void* data, uint32_t dataSize)
{
	void * dest = ((char*)ptr + offset);
	memcpy_s(dest, dataSize, data, dataSize);

	offset += dataSize;

	return offset;
}

void registerForms()
{
	uint32_t tag = 'MENU';
	static MenuForm menuForm = MenuForm();
	holder[tag] = &menuForm;

	tag = 'SCNE';
	static SceneForm sceneForm = SceneForm();
	holder[tag] = &sceneForm;
}

BaseForm::~BaseForm()
{
}

BaseForm * BaseForm::createFromHeader(FormHeader hdr, void * data)
{
	char* data2 = hdr.tag;
	uint32_t tag = (data2[3] << 0) | (data2[2] << 8) | (data2[1] << 16) | (data2[0] << 24);
	
	if (holder.count(tag))
	{
		BaseForm* b = holder.at(tag);
		BaseForm* form = b->createFromData(data, hdr.formSize - sizeof(FormHeader));

		memcpy(form->tag, hdr.tag, 4);
		form->formID = hdr.formID;
		form->formSize = hdr.formSize;
		return form;
	}

	return nullptr;
}

void BaseForm::updateFormID()
{
}

uint32_t BaseForm::getFormID()
{
	return formID;
}
