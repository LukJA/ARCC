#include "pch.h"
#include "generic.h"

using namespace Platform;


#include <vector>
#include <codecvt>

#include <string>



std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;


// -- for use in textblock to int
long ToLongS(String^ str) {
	const wchar_t* begin = str->Data();
	return std::wcstol(begin, nullptr, 10);
}

// -- for use in text to int
long ToLong(std::string str) {
	return std::stoi(str, nullptr, 10);
}

// std string to platform string
String^ toPlat(std::string str) {
	std::wstring wide = converter.from_bytes(str);	//string to wstring
	return ref new String(wide.c_str());	//convert the wstring back to a platform
}

std::string toCstr(String^ str) {
	std::wstring wide = str->Data();	//string to wstring
	return converter.to_bytes(wide);	//convert the wstring back to a platform
}