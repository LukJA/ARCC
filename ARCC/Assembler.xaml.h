//
// Assembler.xaml.h
// Declaration of the Assembler class
//

#pragma once

#include "Assembler.g.h"
#include "MainPage.xaml.h"

namespace ARCC
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Assembler sealed
	{
	public:
		Assembler();

	private:
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
