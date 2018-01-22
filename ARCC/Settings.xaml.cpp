//
// Settings.xaml.cpp
// Implementation of the Settings class
//

#include "pch.h"
#include "Settings.xaml.h"

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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

Settings::Settings()
{
	InitializeComponent();
	this->NavigationCacheMode = Windows::UI::Xaml::Navigation::NavigationCacheMode::Enabled;
}


void ARCC::Settings::ThemeSetting(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
