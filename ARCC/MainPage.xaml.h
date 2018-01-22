//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once
#include "MainPage.g.h"

#include "Compiler.xaml.h"
#include "Assembler.xaml.h"
#include "Editor.xaml.h"
#include "Debug.xaml.h"
#include "Settings.xaml.h"

#include <string>


namespace ARCC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:

		void burgerclick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuCompiler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuAssembler(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuEditor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuDebug(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MenuSettings(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}

