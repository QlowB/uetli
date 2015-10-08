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

#include "Assemblyx86_64.h"
#include <sstream>

using namespace uetli::assembly::x86_64;

std::string registerNames[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rbp",
    "rsi",
    "rdi",
    "rsp",

    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",

    "mmx0",
    "mmx1",
    "mmx2",
    "mmx3",
    "mmx4",
    "mmx5",
    "mmx6",

    "FPR0",
    "FPR1",
    "FPR2",
    "FPR3",
    "FPR4",
    "FPR5",
    "FPR6",

    "XMM0",
    "XMM1",
    "XMM2",
    "XMM3",
    "XMM4",
    "XMM5",
    "XMM6",
    "XMM7",

    "XMM8",
    "XMM9",
    "XMM10",
    "XMM11",
    "XMM12",
    "XMM13",
    "XMM14",

    "rflags",
    "rip"
};

namespace uetli {
namespace assembly {
namespace x86_64 {

const std::string& getRegisterName(Register reg)
{
    return registerNames[reg];
}

}
}
}


RegisterOperand::RegisterOperand(Register reg) :
    reg(reg)
{
}


const RegisterOperand* RegisterOperand::getRegisterOperand(Register reg)
{
    // TODO implement this more efficiently
    return new RegisterOperand(reg);
}


std::string RegisterOperand::toString(void) const
{
    return getRegisterName(reg);
}


std::string MemoryOperand::toString(void) const
{
    return "[" + getRegisterName(address) + "]";
}


std::string ConstantOperand::toString(void) const
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}


AssemblyInstruction::~AssemblyInstruction(void)
{
}


CallInstruction::CallInstruction(const std::string& labelName) :
    labelName(labelName)
{
}


const std::string& CallInstruction::getLabelName(void) const
{
    return labelName;
}



SourceDestinationInstruction::SourceDestinationInstruction(
        const Source* source,
        const Destination* destionation) :
    source(source),
    destionation(destionation)
{
}


std::string SourceDestinationInstruction::toString(void) const
{
    return getInstruction() + " " +
            destionation->toString() + ", " + source->toString();
}


Mov::Mov(const Source* source,
         const Destination* destionation) :
    SourceDestinationInstruction(source, destionation)
{
}


const std::string& Mov::getInstruction(void) const
{
    static std::string inst = "mov";
    return inst;
}


Add::Add(const Source* source,
         const Destination* destionation) :
    SourceDestinationInstruction(source, destionation)
{
}


const std::string& Add::getInstruction(void) const
{
    static std::string inst = "add";
    return inst;
}

