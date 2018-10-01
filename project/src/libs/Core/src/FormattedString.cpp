/// Internal Includes
#include "Core/FormattedString.hpp"
#include "Core/System.hpp"

/// External Includes

/// Std Includes
#include <memory>

namespace Engine {
	namespace Core {

		FormattedString::FormattedString() : size(0U), usedSize(0U), text(nullptr) {}

		FormattedString::FormattedString(const FormattedString& org) : size(0U), usedSize(0U), text(nullptr) {

			if (org.size) {
				text = new FormattedChar[org.size];
				memcpy(text, org.text, org.size * sizeof(FormattedChar));

				size = org.size;
				usedSize = org.usedSize;

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

		FormattedString::FormattedString(const FormattedChar* _text, size_t _size) : size(0U), usedSize(0U), text(nullptr) {

			size_t stringLength = _size;

			if (stringLength != 0U) {

				size = stringLength;
				usedSize = stringLength;

				text = new FormattedChar[size];

				memcpy(text, _text, size * sizeof(FormattedChar));
			}

		}

		FormattedString::FormattedString(const String& org) : size(0U), usedSize(0U), text(nullptr) {
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

		FormattedString &FormattedString::operator=(const char* _text) {
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
					usedSize = stringLength;
				}
			}
			return *this;
		}

		FormattedString &FormattedString::operator+=(const char* _text) {

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
			return *this;
		}

		FormattedString &FormattedString::operator=(const FormattedString _text) {
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
					usedSize = stringLength;
				}
			} else {
				delete[] text;
				text = nullptr;
				size = 0U;
				usedSize = 0U;
			}
			return *this;
		}

		FormattedString &FormattedString::operator+=(const FormattedString _text) {
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
			return *this;
		}

		FormattedString &FormattedString::operator=(const String _text) {

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
					usedSize = stringLength;
				}
			}
			return *this;
		}

		FormattedString &FormattedString::operator+=(const String _text) {

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
			return *this;
		}

		int FormattedString::indexAt(FormattedChar chr, size_t offset) const {

			int index = -1;

			for (size_t i = offset; i < usedSize; i++) {
				if (text[i].c == chr.c) {
					index = (int)i;
					break;
				}
			}
			return index;
		}

		FormattedString FormattedString::subString(size_t offset, size_t count) const {

			if (count > usedSize) {
				count = usedSize - offset;
			}
			if (count == 0) {
				count = usedSize - offset;
			}

			FormattedChar* fChars = new FormattedChar[count];

			for (size_t i = 0; i < count; i++) {
				fChars[i] = text[offset + i];
			}

			FormattedString fString(fChars, count);
			delete fChars;

			return fString;
		}

		void FormattedString::insertAt(int position, FormattedChar chr) {

			if (usedSize + 1 > size) {

				FormattedChar* chars = new FormattedChar[usedSize + 1];

				for (size_t i = 0; i < size_t(position); i++) {
					chars[i] = text[i];
				}
				chars[position] = chr;
				for (size_t i = position; i < usedSize; i++) {
					chars[i + 1] = text[i];
				}

				delete[] text;
				text = chars;
				usedSize++;
				size = usedSize;
			} else {

				for (size_t i = usedSize; i > size_t(position); i--) {
					text[i] = text[i - 1];
				}
				text[position] = chr;
				usedSize++;
			}
		}

		void FormattedString::remoteAt(int position) {

			for (size_t i = position; i < usedSize; i++) {
				text[i] = text[i + 1];
			}
			usedSize--;

		}

		String FormattedString::toString() const {

			String str = "";

			for (size_t i = 0; i < usedSize; i++) {
				str += text[i].c;
			}

			return str;
		}

		void FormattedString::formattedCharMemcpy(FormattedChar* dst, const char* src, const size_t in_size) {

			for (size_t i = 0; i < in_size; i++) {
				dst[i].c = src[i];
				dst[i].r = dst[i].g = dst[i].b = 255;
			}
		}

	}
}