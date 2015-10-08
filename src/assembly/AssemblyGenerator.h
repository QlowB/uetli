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
#include <string>

#include "Assemblyx86_64.h"

#include "../code/StackMachine.h"
#include "../util/HashMap.h"

namespace uetli
{
    namespace assembly
    {
        class AssemblySubroutine;
        class AssemblyGenerator;
    }
}


class uetli::assembly::AssemblySubroutine
{
    std::vector<x86_64::AssemblyInstruction*> instructions;
public:

    AssemblySubroutine(const uetli::code::DirectSubroutine* subroutine);
    std::string toString(void) const;
private:
    void generate(const uetli::code::DirectSubroutine* subroutine);
    void generateInstruction(const uetli::code::StackInstruction* inst);
};


class uetli::assembly::AssemblyGenerator
{
    std::vector<AssemblySubroutine*> subroutines;
    std::string assemblerCmd;
public:
    AssemblyGenerator(void);

    void generateAssembly(const uetli::code::DirectSubroutine* subroutine);

    void assemble(const std::string& outputPath) const;
};


#endif // UETLI_CODE_ASSEMBLYGENERATOR_H_

