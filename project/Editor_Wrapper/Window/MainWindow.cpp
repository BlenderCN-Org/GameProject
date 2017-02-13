#include "MainWindow.hpp"

void MainWindowWrapper::DoWork() {
	window = gcnew Editor::MainWindow();
	
	window->Show();

	System::Windows::Threading::Dispatcher::Run();
}

MainWindowWrapper::MainWindowWrapper() {
	
	thrd = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &MainWindowWrapper::DoWork));
	
	thrd->SetApartmentState(Threading::ApartmentState::STA);
	thrd->IsBackground = true;
	thrd->Start();

}

MainWindowWrapper::~MainWindowWrapper() {
	window->Close();
	thrd->Abort();
}
