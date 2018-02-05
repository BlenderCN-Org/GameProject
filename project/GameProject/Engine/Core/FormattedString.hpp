#ifndef FORMATTED_STRING_HPP
#define FORMATTED_STRING_HPP

/// Internal Includes
#include "String.hpp"

/// External Includes
#include <glm/glm.hpp>

namespace Engine {
	namespace Core {

		struct FormattedChar {
			FormattedChar() : c(0), r(0), g(0), b(0) {}

			FormattedChar(char _c) {
				c = _c;
				r = 255;
				g = 255;
				b = 255;
			}

			operator char() {
				return c;
			}

			operator glm::vec4() {
				return glm::vec4(float(r) / 255.0F, float(g) / 255.0F, float(b) / 255.0F, 1.0F);
			}

			bool operator== (const char chr) {
				if (c == chr) {
					return true;
				}
				return false;
			}

			bool operator== (const FormattedChar chr) {
				if (c == chr.c && r == chr.r &&
					g == chr.g && b == chr.b) {
					return true;
				}
				return false;
			}

			void operator=(const glm::vec4 color) {
				r = unsigned char(color.r * 255.0F);
				g = unsigned char(color.g * 255.0F);
				b = unsigned char(color.b * 255.0F);
			}

			char c;
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};

		class FormattedString {

		public:

			FormattedString();
			FormattedString(const FormattedString& org);
			FormattedString(const char* text);
			FormattedString(const FormattedChar* text, size_t size);
			FormattedString(const String& str);

			virtual ~FormattedString();

			const size_t getSize() const;

			const FormattedChar* cStr() const;

			void formatString(size_t start, size_t end, glm::vec4 color);

			FormattedString &operator=(const char* text);
			FormattedString &operator+=(const char* text);

			FormattedString &operator=(const FormattedString text);
			FormattedString &operator+=(const FormattedString text);

			FormattedString &operator=(const String text);
			FormattedString &operator+=(const String text);

			int indexAt(FormattedChar chr, size_t offset = 0) const;
			FormattedString subString(size_t offset, size_t count) const;

			void insertAt(int position, FormattedChar chr);
			void remoteAt(int position);

		private:

			size_t size;
			size_t usedSize;

			FormattedChar* text;

			void formattedCharMemcpy(FormattedChar* dest, const char* src, const size_t size);

		};

		FormattedString toFString(uint32_t value, uint8_t charPadding);

	}
}

#endif