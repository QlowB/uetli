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


RegisterOperand* RegisterOperand::registers[registers_count] =
{
};


const RegisterOperand* RegisterOperand::getRegisterOperand(Register reg)
{
    static bool initialized = false;

    if (!initialized) {
        for (size_t i = 0; i < sizeof(registers) / sizeof(RegisterOperand*);
             i++) {
            registers[i] = new RegisterOperand((Register) i);
        }
        initialized = true;
    }

    if (reg >= 0 || reg < registers_count)
        return registers[reg];
    else
        throw "fatal internal error";
}


std::string RegisterOperand::toString(void) const
{
    return getRegisterName(reg);
}


MemoryOperand::MemoryOperand(Register address) :
    address(address),
    offset(RAX),
    offsetMultiplier(0),
    immediateOffset(0)
{
}


MemoryOperand::MemoryOperand(Register address, long long immediateOffset) :
    address(address),
    offset(RAX),
    offsetMultiplier(0),
    immediateOffset(immediateOffset)
{
}


MemoryOperand::MemoryOperand(Register address, Register offset,
                             char offsetMultiplier, long long immediateOffset) :
    address(address),
    offset(offset),
    offsetMultiplier(offsetMultiplier),
    immediateOffset(immediateOffset)
{
}


std::string MemoryOperand::toString(void) const
{
    if (immediateOffset != 0 && offsetMultiplier == 0) {
        std::stringstream str;
        str << "[" << getRegisterName(address) << "+" << immediateOffset << "]";
        return str.str();
    }
    else if (offsetMultiplier != 0) {
        std::stringstream str;
        str << "[" + getRegisterName(address) << "+" << getRegisterName(offset)
            << "*" << offsetMultiplier << "+" << immediateOffset << "]";
        return str.str();
    }
}


std::string ConstantOperand::toString(void) const
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}


AssemblyInstruction::AssemblyInstruction(const std::string& instruction) :
    instruction(instruction)
{
}


AssemblyInstruction::~AssemblyInstruction(void)
{
}


std::string AssemblyInstruction::toString(void) const
{
    return instruction;
}


NoArgumentInstruction::NoArgumentInstruction(const std::string& instruction) :
    AssemblyInstruction(instruction)
{
}


Ret::Ret(void) :
    NoArgumentInstruction("ret")
{
}


SingleRegisterInstruction::SingleRegisterInstruction(
        const std::string& instruction, const RegisterOperand* reg) :
    AssemblyInstruction(instruction), reg(reg)
{
}


std::string SingleRegisterInstruction::toString(void) const
{
    return instruction + " " + reg->toString();
}


Push::Push(const RegisterOperand* reg) :
    SingleRegisterInstruction("push", reg)
{
}


Pop::Pop(const RegisterOperand* reg) :
    SingleRegisterInstruction("pop", reg)
{
}


Call::Call(const std::string& labelName) :
    AssemblyInstruction("call"),
    labelName(labelName)
{
}


const std::string& Call::getLabelName(void) const
{
    return labelName;
}


std::string Call::toString(void) const
{
    return instruction + " " + ".text:" + labelName;
}


SourceDestinationInstruction::SourceDestinationInstruction(
        const std::string& instruction,
        const Source* source,
        const Destination* destionation) :
    AssemblyInstruction(instruction),
    source(source),
    destionation(destionation)
{
}


std::string SourceDestinationInstruction::toString(void) const
{
    return instruction + " " +
            destionation->toString() + ", " + source->toString();
}


Mov::Mov(const Source* source,
         const Destination* destionation) :
    SourceDestinationInstruction("mov", source, destionation)
{
}


Add::Add(const Source* source,
         const Destination* destionation) :
    SourceDestinationInstruction("add", source, destionation)
{
}


