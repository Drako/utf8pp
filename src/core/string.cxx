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

#include "string.hxx"

#include <algorithm>
#include <stdexcept>
#include <iostream>

#include <cstring>

namespace
{
	// calculates the next UTF-8 character of the input byte stream
	// safe version -> checks for invalid characters
	utf8::value_t next_s(utf8::byte_t const * strm = nullptr, unsigned length = -1)
	{
		static utf8::byte_t const * ptr = nullptr;
		static unsigned len = 0;
		if (strm)
		{
			ptr = strm;
			len = length;
		}

		// end of string
		if (!len)
			return utf8::END_OF_STRING;

		utf8::value_t result;
		unsigned charlen;

		// single byte character
		if (!(ptr[0] & 0x80))
		{
			charlen = 1;
			result = ptr[0] & 0x7F;
		}
		else
		{
			utf8::byte_t mask;
			// determine the byte length of the character
			if ((ptr[0] & 0xE0) == 0xC0)
				charlen = 2, mask = 0x1F;
			else if ((ptr[0] & 0xF0) == 0xE0)
				charlen = 3, mask = 0x0F;
			else if ((ptr[0] & 0xF8) == 0xF0)
				charlen = 4, mask = 0x07;
			else
				return utf8::INVALID_CHARACTER;

			// character would need more bytes than available
			if (charlen > len)
				return utf8::INVALID_CHARACTER;

			// calculate the unicode index
			result = ptr[0] & mask;
			for (unsigned byte = 1; byte < charlen; ++byte)
			{
				utf8::byte_t cb = ptr[byte];
				if ((cb & 0xC0) != 0x80)
					return utf8::INVALID_CHARACTER;
				result = (result << 6) | (cb & 0x3F);
			}
		}
		// increment ptr and decrement len
		ptr += charlen;
		len -= charlen;

		return result;
	}

	// calculates the next UTF-8 character of the input byte stream
	// unsafe version -> expects string to be valid
	utf8::value_t next(utf8::byte_t const * & strm, unsigned & length)
	{
		// end of string
		if (!length)
			return utf8::END_OF_STRING;

		utf8::value_t result;
		unsigned charlen;

		// single byte character
		if (!(strm[0] & 0x80))
		{
			charlen = 1;
			result = strm[0] & 0x7F;
		}
		else
		{
			utf8::byte_t mask;
			// determine the byte length of the character
			if ((strm[0] & 0xE0) == 0xC0)
				charlen = 2, mask = 0x1F;
			else if ((strm[0] & 0xF0) == 0xE0)
				charlen = 3, mask = 0x0F;
			else if ((strm[0] & 0xF8) == 0xF0)
				charlen = 4, mask = 0x07;

			// calculate the unicode index
			result = strm[0] & mask;
			for (unsigned byte = 1; byte < charlen; ++byte)
			{
				utf8::byte_t cb = strm[byte];
				result = (result << 6) | (cb & 0x3F);
			}
		}
		// increment ptr and decrement len
		strm += charlen;
		length -= charlen;

		return result;
	}

	utf8::value_t fast_foward(utf8::byte_t const * & strm, unsigned & length, unsigned offset)
	{
		if (!offset)
			return utf8::INVALID_CHARACTER;

		for (unsigned n = 1; n < offset; ++n)
		{
			if (strm[0] & 0x80)
				++strm, --length;
			else if ((strm[0] & 0xE0) == 0xC0)
				strm += 2, length -= 2;
			else if ((strm[0] & 0xF0) == 0xE0)
				strm += 3, length -= 3;
			else if ((strm[0] & 0xF8) == 0xF0)
				strm += 4, length -= 4;
			else if (strm[0] == 0)
				return utf8::END_OF_STRING;
		}
		return next(strm, length);
	}
}

namespace utf8
{
	namespace detail
	{
		iterator::iterator()
			: strm_(nullptr)
			, length_(0)
			, value_(INVALID_CHARACTER)
		{
		}

		iterator::~iterator()
		{
		}

		iterator::iterator(byte_t const * strm, unsigned length)
			: strm_(strm)
			, length_(length)
			, value_(next(strm, length))
		{
			if (value_ == END_OF_STRING)
				strm_ = nullptr;
		}

		iterator::iterator(iterator const & src)
			: strm_(src.strm_)
			, length_(src.length_)
			, value_(src.value_)
		{
		}

		iterator & iterator::operator = (iterator const & src)
		{
			strm_ = src.strm_;
			length_ = src.length_;
			value_ = src.value_;
			return *this;
		}

		bool iterator::operator == (iterator const & rhs) const
		{
			// it should be sufficient to check if the byte streams are equal
			return (strm_ == rhs.strm_);
		}

		bool iterator::operator != (iterator const & rhs) const
		{
			return (strm_ != rhs.strm_);
		}

		iterator iterator::operator ++ (int)
		{
			iterator tmp(*this);
			value_ = next(strm_, length_);

			if (value_ == END_OF_STRING)
				strm_ = nullptr;

			return tmp;
		}

		iterator & iterator::operator++()
		{
			value_ = next(strm_, length_);

			if (value_ == END_OF_STRING)
				strm_ = nullptr;

			return *this;
		}

		value_t const & iterator::operator * () const
		{
			return value_;
		}
        
        iterator iterator::operator + (unsigned offset)
		{
			return (iterator(*this) += offset);
		}

		iterator & iterator::operator += (unsigned offset)
		{
			value_t tmp = fast_foward(strm_, length_, offset);
			if (tmp != INVALID_CHARACTER)
				value_ = tmp;

			if (value_ == END_OF_STRING)
				strm_ = nullptr;

			return *this;
		}
	}

	string::string(char const * str /* = nullptr */)
		: length_(0)
	{
		if (str)
		{
			// calculate buffer size
			unsigned bufflen = static_cast<unsigned>(std::strlen(str));

			// calculate length of string
			value_t val = ::next_s(reinterpret_cast<byte_t const *>(str), bufflen);
			while ((val != INVALID_CHARACTER) && (val != END_OF_STRING))
			{
				++length_;
				val = ::next_s();
			}

			// error handling
			if (val == INVALID_CHARACTER)
				throw std::invalid_argument("String contains non UTF-8 bytes.");

			// copy string
			data_.resize(bufflen);
			std::memcpy(&(data_[0]), str, bufflen);
		}
	}

	string::~string()
	{
		// default destructor
	}

	// copy constructot
	string::string(string const & src)
		: length_(src.length_)
		, data_(src.data_)
	{
	}

	// move constructor
	string::string(string && src)
		: length_(src.length_)
	{
		data_.swap(src.data_);
	}

	unsigned string::length() const
	{
		return length_;
	}

	// assignment operator
	string & string::operator = (string const & src)
	{
		data_ = src.data_;
		length_ = src.length_;
		return *this;
	}

	std::vector<byte_t> const & string::data() const
	{
		return data_;
	}

	std::vector<byte_t> & string::data()
	{
		return data_;
	}

	string::const_iterator string::begin() const
	{
		return const_iterator(&(data_[0]), length_);
	}

	string::const_iterator string::end() const
	{
		return const_iterator();
	}
}

std::ostream & operator << (std::ostream & os, utf8::string const & str)
{
	return os.write(reinterpret_cast<char const *>(&((str.data())[0])), str.data().size());
}
