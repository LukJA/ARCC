//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once
#include "MainPage.g.h"

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
		void debugConsole(std::string in);
		void debugConsole(std::wstring wIn);
		void debugConsole(Platform::String ^ pIn);
		void closeClicked(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e);
		void button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void setGen(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void dynamicRealloc(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadIx(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void decodeIx(Platform::String ^ file);
		void fillLineNo(int len);
		void openIQ(Platform::String ^ file, Platform::String^ name);
		void LoadIS(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void VC(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e);
		void OpenAsmFile(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void openASM(Platform::String ^ file);
		void SaveASM(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void generateMC(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void fileGenOT(std::string name);
		void Mcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MUcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Bcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void BUcheck(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void displayErrorBox(Platform::String ^ ErrorCode);
	};
}
