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


#ifndef UETLI_CODE_ASSEMBLYX86_64_H_
#define UETLI_CODE_ASSEMBLYX86_64_H_

#include <string>

namespace uetli
{
    namespace assembly
    {
        namespace x86_64
        {
            ///
            /// \brief word size of the x64 architecture in bytes
            ///
            const int wordSize = 8;

            ///
            /// \brief serves as an identifier for each register of the x64
            ///        architecture
            ///
            enum Register
            {
                // general purpose registers
                RAX = 0,
                RCX,
                RDX,
                RBX,
                RBP,
                RSI,
                RDI,
                RSP,

                R8,
                R9,
                R10,
                R11,
                R12,
                R13,
                R14,
                R15,

                MMX0,
                MMX1,
                MMX2,
                MMX3,
                MMX4,
                MMX5,
                MMX6,

                FPR0,
                FPR1,
                FPR2,
                FPR3,
                FPR4,
                FPR5,
                FPR6,

                XMM0,
                XMM1,
                XMM2,
                XMM3,
                XMM4,
                XMM5,
                XMM6,
                XMM7,

                XMM8,
                XMM9,
                XMM10,
                XMM11,
                XMM12,
                XMM13,
                XMM14,

                RFLAGS,
                RIP,

                /// contains number of registers
                registers_count
            };


            const std::string& getRegisterName(Register reg);

            class Operand;

            class Source;
            class Destination;

            class RegisterOperand;
            class MemoryOperand;
            class ConstantOperand;

            class AssemblyInstruction;
                class NoArgumentInstruction;
                    class Ret;
                class SingleRegisterInstruction;
                    class Push;
                    class Pop;
                class Call;
                class SourceDestinationInstruction;
                    class Mov;
                    class Add;
        }
    }
}



class uetli::assembly::x86_64::Operand
{
public:
    virtual std::string toString(void) const = 0;
};


class uetli::assembly::x86_64::Source : virtual public Operand
{
public:
    virtual std::string toString(void) const = 0;
};


class uetli::assembly::x86_64::Destination : virtual public Operand
{
public:
    virtual std::string toString(void) const = 0;
};


class uetli::assembly::x86_64::RegisterOperand :
        public Source,
        public Destination
{
    Register reg;
    RegisterOperand(Register reg);
    static RegisterOperand* registers[registers_count];
public:
    static const RegisterOperand* getRegisterOperand(Register reg);

    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::MemoryOperand :
        public Source,
        public Destination
{
    Register address;
    Register offset;
    /// must either be 0, 1, 2, 4 or 8
    char offsetMultiplier;
    long long immediateOffset;
public:
    MemoryOperand(Register address);
    MemoryOperand(Register address, long long immediateOffset);
    MemoryOperand(Register address, Register offset, char offsetMultiplier,
                  long long immediateOffset);
    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::ConstantOperand :
        public Source,
        public Destination
{
    unsigned long long value;

public:
    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::AssemblyInstruction
{
protected:
    std::string instruction;
public:
    AssemblyInstruction(const std::string& instruction);
    virtual ~AssemblyInstruction(void);

    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::NoArgumentInstruction :
        public AssemblyInstruction
{
public:
    NoArgumentInstruction(const std::string& instruction);
};


class uetli::assembly::x86_64::Ret :
        public NoArgumentInstruction
{
public:
    Ret(void);
};


class uetli::assembly::x86_64::SingleRegisterInstruction :
        public AssemblyInstruction
{
protected:
    const RegisterOperand* reg;
public:
    SingleRegisterInstruction(const std::string& instruction,
            const RegisterOperand* reg);

    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::Push :
        public SingleRegisterInstruction
{
public:
    Push(const RegisterOperand* reg);
};


class uetli::assembly::x86_64::Pop :
        public SingleRegisterInstruction
{
public:
    Pop(const RegisterOperand* reg);
};


class uetli::assembly::x86_64::Call : public AssemblyInstruction
{
    std::string labelName;
public:
    Call(const std::string& labelName);
    const std::string& getLabelName(void) const;

    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::SourceDestinationInstruction :
        public AssemblyInstruction
{
    const Source* source;
    const Destination* destionation;

public:
    SourceDestinationInstruction(const std::string& instruction,
                                 const Source* source,
                                 const Destination* destionation);

    virtual std::string toString(void) const;

};


class uetli::assembly::x86_64::Mov : public SourceDestinationInstruction
{
public:
    Mov(const Source* source,
        const Destination* destionation);
};


class uetli::assembly::x86_64::Add : public SourceDestinationInstruction
{
public:
    Add(const Source* source,
        const Destination* destionation);
};



#endif // UETLI_CODE_ASSEMBLYX86_64_H_

