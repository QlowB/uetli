// =============================================================================
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
// =============================================================================

#include "Identifier.h"


using namespace uetli::parser;


Identifier::Identifier(void)
{
}


Identifier::Identifier(const std::string& first)
{
    segments.push_back(first);
}


Identifier& Identifier::append(const std::string& segment)
{
    segments.push_back(segment);
    return *this;
}


std::string Identifier::getAsString(void) const
{
    std::string identifier;
    for (size_t i = 0; i < segments.size(); i++) {
        identifier += segments[i];
        if (i + 1 < segments.size())
            identifier += "::";
    }
    return identifier;
}


std::string Identifier::getAssemblySymbol(void) const
{
    std::string identifier;
    for (size_t i = 0; i < segments.size(); i++) {
        const std::string& seg = segments[i];
        if (seg == "*") {
            identifier += "ASTERISK";
        }
        else if (seg == "-") {
            identifier += "MINUS";
        }
        else if (seg == "+") {
            identifier += "PLUS";
        }
        else {
            identifier += segments[i];
        }

        if (i + 1 < segments.size())
            identifier += "__";
    }
    return identifier;
}


