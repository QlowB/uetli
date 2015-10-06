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


#ifndef UETLI_CODE_ASSEMBLYGENERATOR_H_
#define UETLI_CODE_ASSEMBLYGENERATOR_H_

#include <vector>

#include "Assemblyx86_64.h"

#include "../code/StackMachine.h"

namespace uetli
{
    namespace assembly
    {
        class AssemblyGenerator;
    }
}


class uetli::assembly::AssemblyGenerator
{
    const uetli::code::DirectSubroutine* subroutine;
    std::vector<x86_64::AssemblyInstruction*> instructions;
public:
    AssemblyGenerator(const uetli::code::DirectSubroutine* subroutine);

    void generateAssembly(void);

    const std::vector<x86_64::AssemblyInstruction*>&
        getInstructions(void) const;
};


#endif // UETLI_CODE_ASSEMBLYGENERATOR_H_

