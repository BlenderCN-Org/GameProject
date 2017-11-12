#ifndef CONSOLE_HPP
#define CONSOLE_HPP

namespace Engine {
	namespace Core {

		class Console {
		public:

			Console();
			virtual ~Console();

			void setVisible(bool visible);

			bool isVisible() const;

		private:

			bool consoleVisible;

		};
	}
}

#endif