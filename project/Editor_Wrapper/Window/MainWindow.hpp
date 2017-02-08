#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "../CoreInclude.hpp"

ref class MainWindowWrapper
{
public:
	Editor::MainWindow ^window;

	MainWindowWrapper();
	~MainWindowWrapper();
};

#endif

/*
cpp file

ctor() {
	referenced = gcnew myClass();
}
dtor() {
	// or not used at all
	delete referenced;
	// use reference->Close(); if window to make sure the window closes correctly
}
*/