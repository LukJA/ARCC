﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "MainPage.xaml.h"

void ::ARCC::MainPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::ARCC::MainPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->spv = safe_cast<::Windows::UI::Xaml::Controls::SplitView^>(__target);
            }
            break;
        case 2:
            {
                this->settings = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->settings))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::MenuSettings);
            }
            break;
        case 3:
            {
                this->HamburgerButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->HamburgerButton))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::burgerclick);
            }
            break;
        case 4:
            {
                this->MenuButton4 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->MenuButton4))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::MenuDebug);
            }
            break;
        case 5:
            {
                this->MenuButton3 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->MenuButton3))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::MenuEditor);
            }
            break;
        case 6:
            {
                this->MenuButton2 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->MenuButton2))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::MenuAssembler);
            }
            break;
        case 7:
            {
                this->MenuButton1 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->MenuButton1))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ARCC::MainPage::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MainPage::MenuCompiler);
            }
            break;
        case 8:
            {
                this->Cbar = safe_cast<::Windows::UI::Xaml::Controls::CommandBar^>(__target);
            }
            break;
        case 9:
            {
                this->LaFrame = safe_cast<::Windows::UI::Xaml::Controls::Frame^>(__target);
            }
            break;
        case 10:
            {
                this->Lbar = safe_cast<::Windows::UI::Xaml::Shapes::Rectangle^>(__target);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::ARCC::MainPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


