//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//
#include "pch.h"
#include "MainPage.xaml.h"

#include <generic.h>

//#include <memory>
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Platform::Collections;
using namespace concurrency;
using namespace std;

using namespace ARCC;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;

#include <fstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>



MainPage::MainPage()
{
	InitializeComponent();
	LaFrame->Navigate(TypeName(Assembler::typeid));
}


void ARCC::MainPage::burgerclick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	spv->IsPaneOpen = !spv->IsPaneOpen;
}


void ARCC::MainPage::MenuCompiler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LaFrame->Navigate(TypeName(Compiler::typeid));
}


void ARCC::MainPage::MenuAssembler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LaFrame->Navigate(TypeName(Assembler::typeid));
}


void ARCC::MainPage::MenuEditor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LaFrame->Navigate(TypeName(Editor::typeid));
}


void ARCC::MainPage::MenuDebug(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LaFrame->Navigate(TypeName(Debug::typeid));
}


void ARCC::MainPage::MenuSettings(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LaFrame->Navigate(TypeName(Settings::typeid));
}
