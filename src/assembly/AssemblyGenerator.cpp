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
using namespace uetli::assembly::x86_64;


const size_t AssemblySubroutine::nCallerSavedGPRegisters = 9;
const Register AssemblySubroutine::callerSavedGPRegisters[] = {
    RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11,
};


AssemblySubroutine::AssemblySubroutine(
        const uetli::code::DirectSubroutine* subroutine) :
    nPushedRegisters(0),
    operationStackSize(0),
    name(subroutine->getName()),
    labelName(subroutine->getName().getAssemblySymbol())
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


const std::string& AssemblySubroutine::getLabelName(void)
{
    return labelName;
}


void AssemblySubroutine::generate(
        const uetli::code::DirectSubroutine* subroutine)
{
    for (size_t i = 0; i < subroutine->getInstructions().size(); i++) {
        generateInstruction(subroutine->getInstructions()[i]);
    }
    instructions.push_back(new Ret());
}


void AssemblySubroutine::generateInstruction(
    const uetli::code::StackInstruction* instruction)
{
    using namespace uetli::code;

    const CallInstruction* callInst = 0;
    const LoadInstruction* loadInst = 0;

    if ((callInst = dynamic_cast<const CallInstruction*>(instruction))) {
        Call* c = new Call(
                    callInst->getSubroutine()->getName().getAssemblySymbol());
        instructions.push_back(c);
    }
    if ((loadInst = dynamic_cast<const LoadInstruction*>(instruction))) {
        Register currentReg = callerSavedGPRegisters[(operationStackSize) %
                nCallerSavedGPRegisters];
        if (operationStackSize >= nCallerSavedGPRegisters) {
            Push* push = new Push(RegisterOperand::getRegisterOperand(
                                      currentReg));
            instructions.push_back(push);
            nPushedRegisters++;
        }

        Source* source = new MemoryOperand (
            RSP, wordSize * (nPushedRegisters + loadInst->getFromTop())
        );
        Mov* m = new Mov(source,
                         RegisterOperand::getRegisterOperand(currentReg)
        );
        instructions.push_back(m);
        operationStackSize++;
    }
}


void AssemblySubroutine::createStackFrame(void)
{
    instructions.push_back(new Push(RegisterOperand::getRegisterOperand(RBP)));
    instructions.push_back(new Mov(
                               RegisterOperand::getRegisterOperand(RSP),
                               RegisterOperand::getRegisterOperand(RBP)));
}


void AssemblySubroutine::destroyStackFrame(void)
{
    instructions.push_back(new Mov(
                               RegisterOperand::getRegisterOperand(RBP),
                               RegisterOperand::getRegisterOperand(RSP)));
    instructions.push_back(new Pop(RegisterOperand::getRegisterOperand(RBP)));
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


void AssemblyGenerator::writeAssembly(std::ostream& file) const
{
    file << ".intel_syntax noprefix\n";

    for (size_t i = 0; i < subroutines.size(); i++) {
        file << subroutines[i]->getLabelName() << ":\n";
        file << subroutines[i]->toString().c_str() << "\n";
    }

    file << "\n";
}


void AssemblyGenerator::assemble(const std::string& outputPath) const
{
    FILE* process = popen((assemblerCmd + " -o " + outputPath).c_str(), "r+");

    if (!process)
        return;


    pclose(process);
}


