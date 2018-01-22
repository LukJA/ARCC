//
// Settings.xaml.h
// Declaration of the Settings class
//

#pragma once

#include "Settings.g.h"
#include "MainPage.xaml.h"

namespace ARCC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Settings sealed
	{
	public:
		Settings();
	private:
		void ThemeSetting(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
