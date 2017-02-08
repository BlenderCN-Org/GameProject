#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "IEditor.hpp"

namespace Editor {

	class Editor : public IEditor {

	public:

		void initializeEditor();
		void releaseEditor();

	private:

	};

}
#endif
