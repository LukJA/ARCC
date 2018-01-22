//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//
#include "pch.h"
#include "MainPage.xaml.h"

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
}
