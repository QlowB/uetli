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


#include "AssemblyGenerator.h"

#include <cstdio>

using namespace uetli::assembly;

AssemblySubroutine::AssemblySubroutine(
        const uetli::code::DirectSubroutine* subroutine)
{
    generate(subroutine);
}


std::string AssemblySubroutine::toString(void) const
{
    std::string result;
    for (size_t i = 0; i < instructions.size(); i++) {
        result += instructions[i]->toString();
        result += "\n";
    }
    return result;
}


void AssemblySubroutine::generate(
        const uetli::code::DirectSubroutine* subroutine)
{
    for (size_t i = 0; i < subroutine->getInstructions().size(); i++) {
        generateInstruction(subroutine->getInstructions()[i]);
    }
}


void AssemblySubroutine::generateInstruction(
    const uetli::code::StackInstruction* instruction)
{
    using uetli::code::CallInstruction;

    const CallInstruction* callInst = 0;
    if ((callInst = dynamic_cast<const CallInstruction*>(instruction))) {
//        uetli::assembly::x86_64::CallInstruction* c = new
//            uetli::assembly::x86_64::CallInstruction(callInst->getName());
//        instructions.push_back(c);
    }
}


AssemblyGenerator::AssemblyGenerator(void) :
    assemblerCmd("as")
{
}


void AssemblyGenerator::generateAssembly(
        const uetli::code::DirectSubroutine* subroutine)
{
    subroutines.push_back(new AssemblySubroutine(subroutine));
}


void AssemblyGenerator::assemble(const std::string& outputPath) const
{
    FILE* process = popen((assemblerCmd + " -o " + outputPath).c_str(), "r+");

    if (!process)
        return;

    fprintf(process, ".intel_syntax noprefix\n");
    fprintf(stdout, ".intel_syntax noprefix\n");

    for (size_t i = 0; i < subroutines.size(); i++) {
        fprintf(process, "%s\n", subroutines[i]->toString().c_str());
        fprintf(stdout, "%s\n", subroutines[i]->toString().c_str());
    }


    fprintf(process, "\n");
    pclose(process);
}


