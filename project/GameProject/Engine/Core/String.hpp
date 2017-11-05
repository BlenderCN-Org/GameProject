#ifndef STRING_HPP
#define STRING_HPP

namespace Engine {
	namespace Core {

		class String {

		public:

			String();
			String(const String& org);
			String(const char* text);

			virtual ~String();

			const size_t getSize() const;

			const char* cStr();

			void operator=(const char* text);
			void operator+=(const char* text);

			void operator=(const String text);
			void operator+=(const String text);
			

		private:

			size_t size;
			size_t usedSize;

			char* text;

		};

	}
}

#endif