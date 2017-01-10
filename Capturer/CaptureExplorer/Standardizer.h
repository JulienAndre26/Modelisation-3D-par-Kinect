#pragma once

#include <string>
#include <codecvt>
using namespace std;
class Convert {

public:
	static string toString(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}
};