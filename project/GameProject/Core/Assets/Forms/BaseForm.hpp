#ifndef BASEFORM_HPP
#define BASEFORM_HPP

#include <cstdint>
#include <memory>
#include <map>

uint32_t copyToPtr(void* ptr, uint32_t offset, void* data, uint32_t dataSize);

struct FormHeader{
	char tag[4];
uint32_t formID;
	uint32_t formSize;
};

class BaseForm : protected FormHeader
{
public:

	virtual ~BaseForm();

	static BaseForm* createFromHeader(FormHeader hdr, void* data);
	void updateFormID();

	/*
	returns the data into pData with a size of formSize
	*/
	virtual uint32_t getFormID();
	virtual uint32_t getFormSize() = 0;
	virtual void getFormData(void * pData) = 0;

	virtual BaseForm* createFromData(void* data, uint32_t size) = 0;

protected:

};

void registerForms();

#endif