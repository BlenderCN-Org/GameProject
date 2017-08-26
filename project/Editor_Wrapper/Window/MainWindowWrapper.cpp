#include "MainWindowWrapper.hpp"

void MainWindowWrapper::DoWork() {
	window = gcnew Editor::MainWindow();

	window->Show();
	mre->Set();

	try {
		System::Windows::Threading::Dispatcher::Run();
	} catch (System::UnhandledExceptionEventArgs^) {
		Console::Write("Something happened!");
	}
}

void MainWindowWrapper::waitForInit() {
	mre->WaitOne();
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
	delete window;
	//window->Close();
	thrd->Sleep(1000);
	thrd->Abort();
}