
/// Internal Includes
#include "Core/String.hpp"
#include "Core/System.hpp"

/// External Includes

/// Std Includes
#include <memory>

namespace Engine {
	namespace Core {

		String::String() : size(0U), usedSize(0U), text(nullptr) {}

		String::String(const char* _text) : size(0U), usedSize(0U), text(nullptr) {
			size_t stringLength = Engine::System::strlen_s(_text);

			if (stringLength != 0U) {

				size = stringLength;
				usedSize = stringLength;

				text = new char[size + 1];

				memcpy(text, _text, size);
				text[size] = '\0';
			}
		}

		String::String(const String& org) : size(0U), usedSize(0U), text(nullptr) {

			if (text) {
				delete[] text;
			}

			size = org.size;
			usedSize = org.usedSize;

			if (size != 0U) {

				text = new char[size + 1];
				memcpy(text, org.text, size);
				text[size] = '\0';
			}

		}

		String::~String() {
			if (text != nullptr) {
				delete[]text;
				text = nullptr;
			}

			size = 0U;
			usedSize = 0U;
		}

		const size_t String::getSize() const {
			return usedSize;
		}

		const char* String::cStr() const {
			return text;
		}

		bool String::contains(const String& str) {

			// search string larger than me

			const size_t seaLen = str.getSize();

			if (seaLen > getSize()) {
				return false;
			}

			bool found = false;
			size_t offset = 0;
			for (size_t i = 0; i < getSize(); i++) {

				if (true == found) {

					size_t seaIdx = (i - offset);
					
					if (text[i] == str.text[seaIdx]) {
						found = true;
						if (seaIdx == seaLen - 1) {
							break;
						}
					} else {
						found = false;
						i -= seaIdx;
					}

				} else {
					if (text[i] == str.text[0]) {
						offset = i;
						size_t remSize = getSize() - i;
						if (seaLen == 1) {
							found = true;
							break;
						}
						if (seaLen > remSize) {
							break;
						}
						found = true;
					}
				}
			}

			return found;
		}

		bool String::contains(const char* str) {

			// search string larger than me

			const size_t seaLen = System::strlen_s(str);

			if (seaLen > getSize()) {
				return false;
			}

			bool found = false;
			size_t offset = 0;
			for (size_t i = 0; i < getSize(); i++) {

				if (true == found) {

					size_t seaIdx = (i - offset);

					if (text[i] == str[seaIdx]) {
						found = true;
						if (seaIdx == seaLen - 1) {
							break;
						}
					} else {
						found = false;
						i -= seaIdx;
					}

				} else {
					if (text[i] == str[0]) {
						offset = i;
						size_t remSize = getSize() - i;
						if (seaLen == 1) {
							found = true;
							break;
						}
						if (seaLen > remSize) {
							break;
						}
						found = true;
					}
				}
			}

			return found;
		}

		void String::operator=(const char* _text) {
			size_t stringLength = Engine::System::strlen_s(_text);
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new char[stringLength + 1];
					memcpy(text, _text, stringLength);
					text[stringLength] = '\0';

					size = usedSize = stringLength;
				} else {
					memset(text, 0, size);
					memcpy(text, _text, stringLength);
				}
			}
		}

		void String::operator+=(const char* _text) {

			size_t stringLength = Engine::System::strlen_s(_text);

			size_t totalSize = stringLength + usedSize;

			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {

					memcpy(&text[usedSize], _text, stringLength);

					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					char* temp = text;

					text = new char[totalSize + 1];
					memcpy(text, temp, usedSize);
					delete temp;
					memcpy(&text[usedSize], _text, stringLength);
					text[totalSize] = '\0';

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}

		void String::operator+=(const char chr) {

			size_t stringLength = 1;

			size_t totalSize = stringLength + usedSize;

			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {

					text[usedSize] = chr;

					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					char* temp = text;

					text = new char[totalSize + 1];
					memcpy(text, temp, usedSize);
					delete temp;
					text[usedSize] = chr;
					text[totalSize] = '\0';

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}

		void String::operator=(const String _text) {

			size_t stringLength = _text.size;
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new char[stringLength + 1];
					memcpy(text, _text.text, stringLength);
					text[stringLength] = '\0';

					size = usedSize = stringLength;
				} else {
					memset(text, 0, size);
					memcpy(text, _text.text, stringLength);
				}
			}
		}

		void String::operator+=(const String _text) {

			size_t stringLength = _text.size;

			size_t totalSize = stringLength + usedSize;

			// we can append to the current array
			if (stringLength != 0) {
				if (size > totalSize) {

					memcpy(&text[usedSize], _text.text, stringLength);

					usedSize = totalSize;
				}
				// we cannot append to the array so we are going to make a new one
				else {
					char* temp = text;

					text = new char[totalSize + 1];
					memcpy(text, temp, usedSize);
					delete temp;
					memcpy(&text[usedSize], _text.text, stringLength);
					text[totalSize] = '\0';

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}
	}
}