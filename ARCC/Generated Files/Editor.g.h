﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class StackPanel;
                ref class TextBox;
                ref class RelativePanel;
                ref class Button;
                ref class ComboBox;
            }
        }
    }
}

namespace ARCC
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class Editor : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::StackPanel^ globalStack;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ stackPanel;
        private: ::Windows::UI::Xaml::Controls::TextBox^ script;
        private: ::Windows::UI::Xaml::Controls::RelativePanel^ topBar;
        private: ::Windows::UI::Xaml::Controls::Button^ button;
        private: ::Windows::UI::Xaml::Controls::Button^ reload;
        private: ::Windows::UI::Xaml::Controls::ComboBox^ width;
        private: ::Windows::UI::Xaml::Controls::Button^ generate;
        private: ::Windows::UI::Xaml::Controls::Button^ Load;
    };
}

