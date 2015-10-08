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

            enum Register
            {
                // general purpose registers
                RAX = 0,
                RBX,
                RCX,
                RDX,
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
                RIP
            };


            const std::string& getRegisterName(Register reg);

            struct Operand;

            struct Source;
            struct Destination;

            struct RegisterOperand;
            struct MemoryOperand;
            struct ConstantOperand;

            struct AssemblyInstruction;
                struct CallInstruction;
                struct SourceDestinationInstruction;
                    struct Mov;
                    struct Add;
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
public:
    static const RegisterOperand* getRegisterOperand(Register reg);

    virtual std::string toString(void) const;
};


class uetli::assembly::x86_64::MemoryOperand :
        public Source,
        public Destination
{
    Register address;

public:
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
public:
    virtual ~AssemblyInstruction(void);

    virtual std::string toString(void) const = 0;
};


class uetli::assembly::x86_64::CallInstruction : public AssemblyInstruction
{
    std::string labelName;
public:
    CallInstruction(const std::string& labelName);
    const std::string& getLabelName(void) const;

};


class uetli::assembly::x86_64::SourceDestinationInstruction :
        public AssemblyInstruction
{
    const Source* source;
    const Destination* destionation;

public:
    SourceDestinationInstruction(const Source* source,
                                 const Destination* destionation);

    virtual std::string toString(void) const;

    virtual const std::string& getInstruction(void) const = 0;
};


class uetli::assembly::x86_64::Mov : public SourceDestinationInstruction
{
public:
    Mov(const Source* source,
        const Destination* destionation);
    virtual const std::string& getInstruction(void) const;
};


class uetli::assembly::x86_64::Add : public SourceDestinationInstruction
{
public:
    Add(const Source* source,
        const Destination* destionation);
    virtual const std::string& getInstruction(void) const;
};



#endif // UETLI_CODE_ASSEMBLYX86_64_H_

