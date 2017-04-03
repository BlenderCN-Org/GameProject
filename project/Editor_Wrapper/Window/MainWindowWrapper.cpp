#include "MainWindowWrapper.hpp"

void MainWindowWrapper::DoWork() {
	window = gcnew Editor::MainWindow();
	
	window->Show();
	mre->Set();
	System::Windows::Threading::Dispatcher::Run();

}

IntPtr MainWindowWrapper::getGameWindowAreaHandle() {
	
	mre->WaitOne();
	return window->getGameWindowAreaHandle();
}

MainWindowWrapper::MainWindowWrapper() {
	
	mre = gcnew Threading::ManualResetEvent(false);
	thrd = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &MainWindowWrapper::DoWork));
	
	thrd->SetApartmentState(Threading::ApartmentState::STA);
	thrd->IsBackground = true;
	thrd->Start();

}

MainWindowWrapper::~MainWindowWrapper() {
	mre->Close();
	window->Close();
	thrd->Abort();
}
