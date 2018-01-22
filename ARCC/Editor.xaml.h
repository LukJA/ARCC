//
// Editor.xaml.h
// Declaration of the Editor class
//

#pragma once

#include "Editor.g.h"
#include "MainPage.xaml.h"

namespace ARCC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Editor sealed
	{
	public:
		Editor();

	private:
		void closeClicked(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e);
		void button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void setGen(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void dynamicRealloc(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadIx(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void decodeIx(Platform::String ^ file);
	};
}
