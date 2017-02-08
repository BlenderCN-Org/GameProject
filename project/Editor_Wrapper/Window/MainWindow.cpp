#include "MainWindow.hpp"

MainWindowWrapper::MainWindowWrapper() {
	
	window = gcnew Editor::MainWindow();

}

MainWindowWrapper::~MainWindowWrapper() {
	window->Close();
}
