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
#include "semantic/TreeBuilder.h"
#include "code/StackCodeGenerator.h"

#include <cstdio>
#include <cstdlib>

using uetli::ConsoleInterface;
using uetli::UetliConsoleInterface;


ConsoleInterface::ConsoleInterface(int argc, char** argv) :
    in(::stdin), out(::stdout), error(::stderr)
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
    for (size_t i = 1; i < arguments.size(); i++) {
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
        else if(arguments[i] != "") { // normal string argument
            if (!inputFiles.empty()) {
                fprintf(error, "multiple source files specified.\n");
                fflush(stderr);
                exit(1);
            }
            inputFiles.push_back(arguments[i]);
            in = fopen(arguments[i].c_str(), "r");
            openedFiles.push_back(in);
        }
    }
}


UetliConsoleInterface::~UetliConsoleInterface(void)
{
    for (size_t i = 0; i < openedFiles.size(); i++) {
        fclose(openedFiles[i]);
    }
    openedFiles.clear();
}


int UetliConsoleInterface::run(void)
{
    ::uetli_parser_in = in;                                              
    ::uetli_parser_error_out = error;
     
    ::uetli_parser_parse();

    for (size_t i = 0; i < openedFiles.size(); i++) {
        fclose(openedFiles[i]);
    }
    openedFiles.clear(); 

    if (parsedClasses == 0) {
        fprintf(out, "Aborting Compilation\n");;
        return 1;
    }


    uetli::semantic::TreeBuilder tb(*parsedClasses);

    tb.build();

    for (size_t i = 0; i < parsedClasses->size(); i++) {
        delete parsedClasses->at(i);
    }
    delete parsedClasses;
    parsedClasses = 0;


    const std::vector<uetli::semantic::EffectiveClass*>& classes =
            tb.getAttributedClasses();

    if (classes.size() > 0) {
        uetli::semantic::EffectiveClass* cl = classes[0];
        fprintf(out, "Parsed at least one class: %s\n", cl->getName().c_str());
        fprintf(out, "%s has %ld methods.\n", cl->getName().c_str(),
                cl->getNMethods());
        
        ;
        if (cl->getNMethods() != 0) {
            fprintf(out, "Parsed at least one method: %s\n", cl->getMethod(0)->
                    getName().c_str());
            uetli::code::StackCodeGenerator scg(cl->getMethod(0));
            scg.generateCode();

            uetli::code::Subroutine* rout = scg.getGeneratedCode();
            fprintf(out, "stack instruction dump:\n%s",
                    rout->toString().c_str());
            /*for (int i = 0; i < rout->instructions.size(); i++) {
                fprintf(out, "instruction: %s\n", uetli::code::getDescription(
                             rout->instructions[i]).c_str());
            }*/
        }
    }


    fprintf(out, "Parsed %ld classes\n", classes.size());

    return 0;
}


