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


#include "Interface.h"
#include "parser/uetli_parser.h"

using uetli::ConsoleInterface;
using uetli::UetliConsoleInterface;


ConsoleInterface::ConsoleInterface(int argc, char** argv)
    in(stdin), out(stdout), error(stderr)
{
    for (int i = 0; i < argc; i++) {
        arguments.push_back(std::string(argv[i]));
    }
}


ConsoleInterface::~ConsoleInterface(void)
{
}


UetliConsoleInterface::UetliConsoleInterface(int argc, char** argv) :
    ConsoleInterface(argc, argv)
{
    for (size_t i = 0; i < arguments.size(); i++) {
        if (arguments[i] == "-o") {           
            if (arguments.size() > i + 1) {
                outputFilename = arguments[i + 1];
                i++;
            }
            else {
                fprintf(stderr, "no output file specified!\n");
                fflush(stderr);
                exit(1);
            }
        }
        else { // normal string argument
            inputFiles.push_back(arguments[i]);
        }
    }
}


void UetliConsoleInterface::run(void)
{
    ::uetli_parser_in = in;                                              
    ::uetli_parser_error_out = error;
    return ::uetli_parser_parse();
}


