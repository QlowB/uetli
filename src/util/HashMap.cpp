// ============================================================================
//
// This file is part of the uetli compiler.
//
// Copyright (C) 2014-2015 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================

#include "HashMap.h"


template<>
size_t DefaultHash<std::string>::hash(const std::string& key)
{
    const size_t prime1 = 14399107;
    const size_t prime2 = 10382231;

    size_t hash = 35809439;

    typedef std::string::const_iterator StringIterator;
    for (StringIterator i = key.begin(); i != key.end(); i++) {
        hash = hash * prime1 ^ (*i) * prime2;
    }
    return hash;
}


uetli::util::NoEntryException::NoEntryException(const std::string &message) :
    message(message)
{
}


uetli::util::NoEntryException::~NoEntryException(void) throw()
{
}


const char* uetli::util::NoEntryException::what(void) const throw()
{
    return message.c_str();
}



