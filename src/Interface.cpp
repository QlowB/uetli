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
#include "assembly/AssemblyGenerator.h"
#include "assembly/Assemblyx86_64.h"

#include <cstdio>
#include <cstdlib>

using uetli::ConsoleInterface;
using uetli::UetliConsoleInterface;


ConsoleInterface::ConsoleInterface(int argc, char** argv)
{
    for (int i = 0; i < argc; i++) {
        arguments.push_back(std::string(argv[i]));
    }
}


ConsoleInterface::~ConsoleInterface(void)
{
}


void ConsoleInterface::printError(const std::string& error)
{
    std::cerr << "\x1B[31;1m" "error:" "\x1b[0m" " " << error << "\n";
}


UetliConsoleInterface::UetliConsoleInterface(int argc, char** argv) :
    ConsoleInterface(argc, argv),
    outputFilename("a.out")
{
    for (size_t i = 1; i < arguments.size(); i++) {
        if (arguments[i] == "-o") {
            if (arguments.size() > i + 1) {
                outputFilename = arguments[i + 1];
                i++;
            }
            else {
                printError("no output file specified");
                fflush(stderr);
                exit(1);
            }
        }
        else if(arguments[i] != "") { // normal string argument
            if (!inputFiles.empty()) {
                printError("multiple source files specified");
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


int UetliConsoleInterface::run(void) throw()
{
    try {
        return runInterface();
    }
    catch (uetli::parser::ParserException& pe) {
        printError(pe.getErrorMessage());
    }
    catch (...) {
        printError("compilation terminated due to fatal error");
    }
    return 1;
}


int UetliConsoleInterface::runInterface(void)
{
    using std::cout;

    ::uetli_parser_in = stdin;

    bool log = true;

    if (log) {
        cout << "starting parsing..." << std::endl;
    }
    
    ::uetli_parser_parse();

    if (log) {
        cout << "done parsing" << std::endl;
    }

    for (size_t i = 0; i < openedFiles.size(); i++) {
        fclose(openedFiles[i]);
    }

    openedFiles.clear(); 

    if (parsedClasses == 0) {
        cout << "Aborting Compilation" << std::endl;
        return 1;
    }


    uetli::semantic::TreeBuilder tb(*parsedClasses);

    tb.build();

    if (log) {
        cout << "built attributed syntax tree." << std::endl;
    }

    for (size_t i = 0; i < parsedClasses->size(); i++) {
        delete parsedClasses->at(i);
    }

    delete parsedClasses;
    parsedClasses = 0;


    const std::vector<uetli::semantic::EffectiveClass*>& classes =
            tb.getAttributedClasses();

    std::vector<uetli::code::DirectSubroutine*> subroutines;

    for (size_t i = 0; i < classes.size();  i++) {
        uetli::semantic::EffectiveClass* cl = classes[i];

        for (size_t j = 0; j < cl->getNMethods(); j++) {
            uetli::semantic::Method* method = cl->getMethod(j);

            uetli::code::StackCodeGenerator scg(method);
            scg.generateCode();

            uetli::code::DirectSubroutine* rout = scg.getGeneratedCode();

            subroutines.push_back(rout);
        }
    }


    uetli::assembly::AssemblyGenerator assemblyGenerator;
    for (size_t i = 0; i < subroutines.size(); i++) {
        assemblyGenerator.generateAssembly(subroutines[i]);
    }

    for (size_t i = 0; i < subroutines.size(); i++) {
        delete subroutines[i];
    }

#if 0
    FILE* output = 0;

    if (outputFilename != "") {
        output = ::fopen(outputFilename.c_str(), "w");
        if (!output) {
            printError(std::string("could not create file: ") + outputFilename);
            exit(1);
        }
    }

    if (!output) {
        for (size_t i = 0; i < subroutines.size(); i++) {
            fprintf(out, "%s\n", subroutines[i]->toString().c_str());
        }
    }
    else {
        for (size_t i = 0; i < subroutines.size(); i++) {
            fprintf(output, "%s\n", subroutines[i]->toString().c_str());
        }
        ::fclose(output);
    }
#endif


    assemblyGenerator.writeAssembly(stdout);


    FILE* assembler =
        ::popen((std::string("as -o ") + outputFilename).c_str(), "w"); // r+

    assemblyGenerator.writeAssembly(assembler);
    ::pclose(assembler);


#if 0
    if (classes.size() > 0) {
        uetli::semantic::EffectiveClass* cl = classes[0];
        fprintf(out, "Parsed at least one class: %s\n", cl->getName().c_str());
        fprintf(out, "%s has %ld methods.\n", cl->getName().c_str(),
                cl->getNMethods());

        if (cl->getNMethods() != 0) {
            fprintf(out, "Parsed at least one method: %s\n", cl->getMethod(0)->
                    getName().c_str());

            uetli::code::StackCodeGenerator scg(cl->getMethod(0));
            scg.generateCode();

            uetli::code::DirectSubroutine* rout = scg.getGeneratedCode();
//            fprintf(out, "stack instruction dump:\n%s\n",
//                    rout->toString().c_str());

            FILE* output = 0;
            if (outputFilename != "")
                output = ::fopen(outputFilename.c_str(), "w");
            if (!output)
                fprintf(out, "%s\n", rout->toString().c_str());
            else {
                fprintf(output, "%s\n", rout->toString().c_str());
                ::fclose(output);
            }




            /*for (int i = 0; i < rout->instructions.size(); i++) {
                fprintf(out, "instruction: %s\n", uetli::code::getDescription(
                             rout->instructions[i]).c_str());
            }*/

//            uetli::assembly::AssemblyGenerator ag;
//            ag.generateAssembly(rout);

//            ag.assemble("obj.o");
        }
    }
#endif

    cout << "Parsed " << classes.size() << " classes" << std::endl;

    return 0;
}


