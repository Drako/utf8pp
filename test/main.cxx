/*
    Copyright (C) 2012, Felix Bytow <drako123@gmx.de>

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

#include "core/string.hxx"

#include <iostream>
#include <iomanip>
#include <algorithm>

int main()
{
    utf8::string name;
    std::cout << "Hello, what is your name? ";
    utf8::getline(std::cin, name);
    std::cout << "So your name is " << name << "\n";
    
    // thats for the number formatting...
    std::cout << std::hex << std::uppercase;
    
    std::cout << "It looks like this in UNICODE: { U+" << std::setfill('0') << std::setw(4) << name[0];
    std::for_each(name.begin() + 1, name.end(), [](utf8::value_t val){
        std::cout << ", U+" << std::setfill('0') << std::setw(4) << val;
    });
    std::cout << " }" << std::endl;
    return 0;
}
