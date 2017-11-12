#include "FormattedString.hpp"
#include "System.hpp"

#include <memory>

namespace Engine {
	namespace Core {

		FormattedString::FormattedString() : size(0U), usedSize(0U), text(nullptr) {}

		FormattedString::FormattedString(const FormattedString& org) {
			if (text) {
				delete[] text;
			}

			size = org.size;
			usedSize = org.usedSize;

			if (size != 0U) {

				text = new FormattedChar[size];
				memcpy(text, org.text, size);

			}
		}

		FormattedString::FormattedString(const char* _text) : size(0U), usedSize(0U), text(nullptr) {
			size_t stringLength = Engine::System::strlen_s(_text);

			if (stringLength != 0U) {

				size = stringLength;
				usedSize = stringLength;

				text = new FormattedChar[size];

				formattedCharMemcpy(text, _text, size);
			}
		}

		FormattedString::FormattedString(const String& org) {
			if (text) {
				delete[] text;
			}

			size = org.getSize();
			usedSize = org.getSize();

			if (size != 0U) {

				text = new FormattedChar[size];
				formattedCharMemcpy(text, org.cStr(), size);

			}
		}

		FormattedString::~FormattedString() {
			if (text != nullptr) {
				delete[]text;
				text = nullptr;
			}

			size = 0U;
			usedSize = 0U;
		}

		const size_t FormattedString::getSize() const {
			return usedSize;
		}

		const FormattedChar* FormattedString::cStr() const {
			return text;
		}

		void FormattedString::formatString(size_t start, size_t end, glm::vec4 color) {

			// make sure we cannot format more than the length of the string
			if (end > usedSize) {
				end = usedSize;
			}

			for (size_t i = start; i < end; i++) {
				text[i] = color;
			}
		}

		void FormattedString::operator=(const char* _text) {
			size_t stringLength = Engine::System::strlen_s(_text);
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new FormattedChar[stringLength];
					formattedCharMemcpy(text, _text, stringLength);
			
					size = usedSize = stringLength;
				} else {
					memset(text, 0, size * sizeof(FormattedChar));
					formattedCharMemcpy(text, _text, stringLength);
				}
			}
		}

		void FormattedString::operator+=(const char* _text) {

			size_t stringLength = Engine::System::strlen_s(_text);
			
			size_t totalSize = stringLength + usedSize;
			
			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {
			
					formattedCharMemcpy(&text[usedSize], _text, stringLength);
			
					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					FormattedChar* temp = text;
			
					text = new FormattedChar[totalSize];
					memcpy(text, temp, usedSize * sizeof(FormattedChar));
					delete temp;
					formattedCharMemcpy(&text[usedSize], _text, stringLength);
			
					size = totalSize;
					usedSize = totalSize;
				}
			}

		}

		void FormattedString::operator=(FormattedString _text) {
			size_t stringLength = _text.getSize();
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new FormattedChar[stringLength];
					memcpy(text, _text.text, stringLength * sizeof(FormattedChar));

					size = usedSize = stringLength;
				} else {
					memset(text, 0, size * sizeof(FormattedChar));
					memcpy(text, _text.text, stringLength * sizeof(FormattedChar));
				}
			}
		}

		void FormattedString::operator+=(FormattedString _text) {
			size_t stringLength = _text.getSize();

			size_t totalSize = stringLength + usedSize;

			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {

					memcpy(&text[usedSize], _text.text, stringLength * sizeof(FormattedChar));

					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					FormattedChar* temp = text;

					text = new FormattedChar[totalSize];
					memcpy(text, temp, usedSize * sizeof(FormattedChar));
					delete temp;
					memcpy(&text[usedSize], _text.text, stringLength * sizeof(FormattedChar));

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}

		void FormattedString::operator=(String _text) {

			size_t stringLength = _text.getSize();
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new FormattedChar[stringLength];
					formattedCharMemcpy(text, _text.cStr(), stringLength);
			
					size = usedSize = stringLength;
				} else {
					memset(text, 0, size * sizeof(FormattedChar));
					formattedCharMemcpy(text, _text.cStr(), stringLength);
				}
			}
		}

		void FormattedString::operator+=(String _text) {

			size_t stringLength = _text.getSize();

			size_t totalSize = stringLength + usedSize;

			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {

					const char* add = _text.cStr();

					formattedCharMemcpy(&text[usedSize], add, stringLength);

					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					FormattedChar* temp = text;

					text = new FormattedChar[totalSize];
					memcpy(text, temp, usedSize * sizeof(FormattedChar));
					delete temp;
					const char* add = _text.cStr();
					formattedCharMemcpy(&text[usedSize], add, stringLength);

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}
		void FormattedString::formattedCharMemcpy(FormattedChar* dst, const char* src, const size_t size) {

			for (size_t i = 0; i < size; i++) {
				dst[i].c = src[i];
				dst[i].r = dst[i].g = dst[i].b = 255;
			}
		}
	}
}