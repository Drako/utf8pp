#pragma once

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

#ifndef UTF8PP_CORE_STRING_HXX
#define UTF8PP_CORE_STRING_HXX

#include <vector>
#include <iosfwd>

namespace utf8
{
	// value_t represents an index in the unicode table
	typedef unsigned int value_t;
	// byte_t represents a single byte
	typedef unsigned char byte_t;

	// The end of string character
	static value_t const END_OF_STRING = 0;
	// An invalid character
	static value_t const INVALID_CHARACTER = -1;

	class string;

	namespace detail
	{
		class iterator
		{
		public:
			iterator();
			~iterator();

			iterator(iterator const & src);
			iterator & operator = (iterator const & src);

			bool operator == (iterator const & rhs) const;
			bool operator != (iterator const & rhs) const;

			iterator operator ++ (int);
			iterator & operator ++ ();

			value_t const & operator * () const; 

		private:
			iterator(byte_t const * strm, unsigned length);

			// same data as used by internal next and next_s
			byte_t const * strm_;
			unsigned length_;

			// the current value
			value_t value_;

			// string class needs access to the constructor
			friend class string;
		};
	}

	// NOTE: utf-8 string's do not contain a NULL byte while the
	// byte stream used to initialize the string might contain one
	class string
	{
	public:
		// This constructor will throw an std::invalid_argument exception
		// if the input string contains invalid bytes.
		string(char const * str = nullptr);
		~string();

		string(string const & src);
		string(string && src);
		string & operator = (string const & src);

		// get the length of the string in characters
		unsigned length() const;
		// access the data directly
		std::vector<byte_t> const & data() const;
		std::vector<byte_t> & data();

		// iterators
		typedef detail::iterator const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

	private:
		// The byte data of the string
		std::vector<byte_t> data_;
		// The length of the string in characters
		unsigned length_;
	};
}

// used to write UTF-8 text to an output stream
std::ostream & operator << (std::ostream & os, utf8::string const & str);

#endif // UTF8PP_CORE_STRING_HXX
