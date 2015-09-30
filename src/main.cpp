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
using namespace uetli;

int main(int argc, char** argv)
{

    ConsoleInterface* interface;

    interface = new UetliConsoleInterface(argc, argv);

    try {
        interface->run();
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



