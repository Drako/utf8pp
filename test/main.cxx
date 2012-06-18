/*
    Copyright (C) 2012, Felix Bytow <felix.bytow@googlemail.com>

    This file is part of utf8pp.

    utf8pp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    utf8pp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with utf8pp.  If not, see <http://www.gnu.org/licenses/>.
*/

#define UTF8PP_TEST

#include "core/string.hxx"
#include "conversion/encoding_cast.hxx"
using utf8::encoding_cast;

#include <iostream>
#include <string>

int main()
{
    //wchar_t euro[] = { 0x20AC, L' ', L'=', L' ', L'e', L'u', L'r', L'o', 0 };
    wchar_t euro[] = L"\u20AC = euro";
    std::wstring eurow(euro);
    
    auto eurou = encoding_cast<utf8::string>(eurow);
    
    std::cout << eurou << "\n";
    
    return 0;
}
