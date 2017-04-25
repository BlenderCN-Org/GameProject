#ifndef MENUFORM_HPP
#define MENUFORM_HPP

#include "BaseForm.hpp"

#define MENU_VISIBLE       0x0001
#define MENU_ITEM_ENABLED  0x0002


struct SubMenuItem {
	uint32_t flags;
	char name[80];
	char command[80];

};

class MenuForm : public BaseForm
{
	friend class AssetManager;

public:

	MenuForm();
	~MenuForm();

	virtual uint32_t getFormSize();
	virtual void getFormData(void * pData);
	virtual BaseForm* createFromData(void* data, uint32_t size);

private:

	uint32_t flags;
	uint32_t nrMenuItems;
	SubMenuItem* subMenuItems = nullptr;

};

#endif