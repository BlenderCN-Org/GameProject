#include "MainWindowWrapper.hpp"

void MainWindowWrapper::DoWork() {
	window = gcnew Editor::MainWindow();
	
	window->Show();

	System::Windows::Threading::Dispatcher::Run();

}

IntPtr MainWindowWrapper::getGameWindowAreaHandle() {
	
	while ( window == nullptr );

	return window->getGameWindowAreaHandle();
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
