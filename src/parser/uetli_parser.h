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

#ifndef UETLI_PARSER_UETLI_PARSER_H_
#define UETLI_PARSER_UETLI_PARSER_H_

#include <vector>
#include <cstdio>

namespace uetli
{
    namespace parser
    {
        struct ClassDeclaration;
    }
}

///
/// \brief contains the parsed classes
///
/// After the parser has parsed the input, its output can be found in this list.
///
extern std::vector<uetli::parser::ClassDeclaration*>* parsedClasses;

////
/// \brief error output stream for the parser
///
/// This stream is used as the error output by the parser.
///
extern FILE* uetli_parser_error_out;

//
/// \brief input stream for the parser
///
/// This stream is used as the code input by the parser.
///
extern FILE* uetli_parser_in;

///
/// \brief the parse function
///
/// This function invokes the actual parser. The result will be stored in
/// parsedClasses.
///
extern void uetli_parser_parse(void);


#endif // UETLI_PARSER_UETLI_PARSER_H_

