#include "MenuForm.hpp"

#include "../../Memory/MemoryBuffer.hpp"
#include <Editor_Wrapper/IEditor.hpp>

MenuForm::MenuForm()
{
	memcpy(tag, "MENU", 4);
	//formID = 1234;
	//flags = MENU_VISIBLE;
	//nrMenuItems = 2;
	//subMenuItems = new SubMenuItem[nrMenuItems];
	//memset(subMenuItems, 0, nrMenuItems * sizeof(SubMenuItem));
	//for (uint32_t i = 0; i < nrMenuItems; i++)
	//{
	//	subMenuItems[i].flags = MENU_ITEM_ENABLED;
	//	strcpy_s(subMenuItems[i].name, "Name :D");
	//	strcpy_s(subMenuItems[i].command, "Command :P");
	//}
}

MenuForm::~MenuForm()
{
	delete [] subMenuItems;
}

uint32_t MenuForm::getFormSize()
{
	formSize = 0;

	formSize += sizeof(tag) + sizeof(formID) + sizeof(formSize);
	formSize += sizeof(flags) + sizeof(nrMenuItems);
	
	uint32_t subSize = sizeof(SubMenuItem::flags) + sizeof(SubMenuItem::name) + sizeof(SubMenuItem::command);
	formSize += (subSize * nrMenuItems);
	
	return formSize;
}

void MenuForm::getFormData(void * pData)
{
	if (pData == nullptr)
		return;
	uint32_t offset = 0;

	offset = copyToPtr(pData, offset, &tag, sizeof(tag));
	offset = copyToPtr(pData, offset, &formID, sizeof(uint32_t));
	offset = copyToPtr(pData, offset, &formSize, sizeof(uint32_t));
	offset = copyToPtr(pData, offset, &flags, sizeof(uint32_t));
	offset = copyToPtr(pData, offset, &nrMenuItems, sizeof(uint32_t));

	for (uint32_t i = 0; i < nrMenuItems; i++)
	{
		offset = copyToPtr(pData, offset, &subMenuItems[i].flags, sizeof(uint32_t));
		offset = copyToPtr(pData, offset, &subMenuItems[i].name, sizeof(SubMenuItem::name));
		offset = copyToPtr(pData, offset, &subMenuItems[i].command, sizeof(SubMenuItem::command));
	}
}

BaseForm * MenuForm::createFromData(void * data, uint32_t size)
{
	MenuForm* form = new MenuForm();
	MemoryBuffer memBuff;
	memBuff.setData(data, size);
	form->flags = *(uint32_t*)memBuff.returnBytes(sizeof(flags));
	form->nrMenuItems = *(uint32_t*)memBuff.returnBytes(sizeof(flags));

	form->subMenuItems = new SubMenuItem[form->nrMenuItems];

	for (size_t i = 0; i < form->nrMenuItems; i++)
	{
		form->subMenuItems[i].flags = *(uint32_t*)memBuff.returnBytes(sizeof(SubMenuItem::flags));
		memcpy(form->subMenuItems[i].name, memBuff.returnBytes(sizeof(SubMenuItem::name)), sizeof(SubMenuItem::name));
		memcpy(form->subMenuItems[i].command, memBuff.returnBytes(sizeof(SubMenuItem::command)), sizeof(SubMenuItem::command));
	}

	memBuff.deleteBuffer();
	return form;
}

uint32_t MenuForm::getType() {
	return 0;
}
