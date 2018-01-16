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
                ref class Pivot;
                ref class Frame;
                ref class RelativePanel;
                ref class TextBlock;
                ref class TextBox;
                ref class StackPanel;
                ref class Button;
                ref class ComboBox;
                ref class ScrollViewer;
                ref class CheckBox;
            }
        }
    }
}

namespace ARCC
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::Pivot^ rootPivot;
        private: ::Windows::UI::Xaml::Controls::Frame^ helpFrame;
        private: ::Windows::UI::Xaml::Controls::RelativePanel^ helpStack;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ tb1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ console;
        private: ::Windows::UI::Xaml::Controls::Frame^ simFrame;
        private: ::Windows::UI::Xaml::Controls::Frame^ insFrame;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ globalStack;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ stackPanel;
        private: ::Windows::UI::Xaml::Controls::TextBox^ script;
        private: ::Windows::UI::Xaml::Controls::RelativePanel^ topBar;
        private: ::Windows::UI::Xaml::Controls::Button^ button;
        private: ::Windows::UI::Xaml::Controls::Button^ reload;
        private: ::Windows::UI::Xaml::Controls::ComboBox^ width;
        private: ::Windows::UI::Xaml::Controls::Button^ generate;
        private: ::Windows::UI::Xaml::Controls::Button^ Load;
        private: ::Windows::UI::Xaml::Controls::Frame^ asmFrame;
        private: ::Windows::UI::Xaml::Controls::RelativePanel^ boqqq;
        private: ::Windows::UI::Xaml::Controls::RelativePanel^ Tools;
        private: ::Windows::UI::Xaml::Controls::ScrollViewer^ in;
        private: ::Windows::UI::Xaml::Controls::ScrollViewer^ LineNo;
        private: ::Windows::UI::Xaml::Controls::ScrollViewer^ out;
        private: ::Windows::UI::Xaml::Controls::TextBox^ outT;
        private: ::Windows::UI::Xaml::Controls::TextBox^ LineNoT;
        private: ::Windows::UI::Xaml::Controls::TextBox^ inT;
        private: ::Windows::UI::Xaml::Controls::Button^ ASSLoad;
        private: ::Windows::UI::Xaml::Controls::Button^ ASSsave;
        private: ::Windows::UI::Xaml::Controls::Button^ ISLoad;
        private: ::Windows::UI::Xaml::Controls::TextBox^ Len;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ fX;
        private: ::Windows::UI::Xaml::Controls::Button^ ASGen;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ MIF;
        private: ::Windows::UI::Xaml::Controls::CheckBox^ BIN;
    };
}

