#include "String.hpp"
#include "System.hpp"

#include <memory>

namespace Engine {
	namespace Core {

		String::String() : size(0U), usedSize(0U), text(nullptr) {}

		String::String(const char* _text) : size(0U), usedSize(0U), text(nullptr) {
			size_t stringLength = Engine::System::strlen_s(_text);

			if (stringLength != 0U) {

				size = stringLength;
				usedSize = stringLength;

				text = new char[size];

				memcpy(text, _text, size);
			}
		}

		String::String(const String& org) {

			if (text) {
				delete[] text;
			}

			size = org.size;
			usedSize = org.usedSize;

			if (size != 0U) {

				text = new char[size];
				memcpy(text, org.text, size);

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

		const char* String::cStr() {
			return text;
		}


		void String::operator=(const char* _text) {
			size_t stringLength = Engine::System::strlen_s(_text);
			if (stringLength != 0) {
				if (stringLength > size) {
					delete[] text;
					text = new char[stringLength];
					memcpy(text, _text, stringLength);

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

					text = new char[totalSize];
					memcpy(text, temp, usedSize);
					memcpy(&text[usedSize], _text, stringLength);

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
					text = new char[stringLength];
					memcpy(text, _text.text, stringLength);

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

					text = new char[totalSize];
					memcpy(text, temp, usedSize);
					memcpy(&text[usedSize], _text.text, stringLength);

					size = totalSize;
					usedSize = totalSize;
				}
			}
		}
	}
}