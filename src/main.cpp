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


#include <iostream>
#include <cstdio>

#include "Interface.h"

using namespace std;


int main(int argc, char** argv)
{

    ConsoleInterface* interface;

    interface = new UetliConsoleInterface(argc, argv);

    try {

        interface->run();

        if (parsedClasses == 0) {
            std::cout << "Aborting Compilation" << std::endl;
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
            std::cout << "Parsed at least one class: " << cl->getName() <<
                         std::endl;
            std::cout << cl->getName() << " has " << cl->getNMethods() <<
                         " methods." << std::endl;
            if (cl->getNMethods() != 0) {
                std::cout << "Parsed at least one method: " <<
                          cl->getMethod(0)->getName() << std::endl;
                uetli::code::StackCodeGenerator scg(cl->getMethod(0));
                scg.generateCode();

                uetli::code::Subroutine* rout = scg.getGeneratedCode();
                for (int i = 0; i < rout->instructions.size(); i++) {
                    std::cout << "instruction: " << uetli::code::getDescription(
                                 rout->instructions[i]) << std::endl;
                }
            }
        }


        std::cout << "Parsed " << classes.size() << " classes\n";

    }
    catch (const char* err) {
        std::cerr << "compilation terminated: " << err << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "compilation terminated!\n";
        return 1;
    }

    return 0;
}



