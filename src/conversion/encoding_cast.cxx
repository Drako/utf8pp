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

#include "encoding_cast.hxx"

namespace utf8
{
    namespace platform
    {
        namespace detail
        {
            template <int WCharSize>
            struct converter;

            template <>
            struct converter<4>
            {
                static std::wstring to_wstring(utf8::string const & src)
                {
                    std::wstring result;
                    result.reserve(src.length());
                    for (auto const & c : src)
                    {
                        result.push_back(c);
                    }
                    return result;
                }

                static utf8::string from_wstring(std::wstring const & src)
                {
                    // TODO: implement UTF-32 to UTF-8 conversion
                    return utf8::string();
                }
            };

            template <>
            struct converter<2>
            {
                static std::wstring to_wstring(utf8::string const & src)
                {
                    // TODO: implement UTF-8 to UTF-16 conversion
                    return std::wstring();
                }

                static utf8::string from_wstring(std::wstring const & src)
                {
                    // TODO: implement UTF-16 to UTF-8 conversion
                    return utf8::string();
                }
            };
        }

        utf8::string wstring2utf8(std::wstring const & src)
        {
            return detail::converter<sizeof(wchar_t)>::from_wstring(src);
        }

        std::wstring utf82wstring(utf8::string const & src)
        {
            return detail::converter<sizeof(wchar_t)>::to_wstring(src);
        }
    }
}
