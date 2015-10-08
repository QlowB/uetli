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

#ifndef UETLI_CODE_STACKMACHINE_H_
#define UETLI_CODE_STACKMACHINE_H_

#include <vector>
#include <string>

namespace uetli
{
    namespace code
    {
        ///
        /// \brief type to reference to elements in the stack
        ///
        typedef unsigned long Word;

        class StackInstruction;
            class LoadInstruction;
            class StoreInstruction;
            class DereferenceInstruction;
            class DereferenceStoreInstruction;
            class PopInstruction;
            class CallInstruction;
            class LoadConstantInstruction;
            class AllocateInstruction;
            class DuplicateInstruction;
            class PrintInstruction;

            class Subroutine;
                class SubroutineLink;
                class DirectSubroutine;

        // only for debug purpose
        std::string getDescription(StackInstruction*);
    }
}


///
/// \brief base class for all instructions
///
/// All from StackInstruction derived classes are designed to operate on a stack
/// machine. These instructions are not really interpreted then, but rather used
/// as an intermediary format for code generation.
///
class uetli::code::StackInstruction
{
public:
    ///
    /// \brief execute the instruction on a stack
    ///
    /// The implementation of this method is used for testing purposes.
    ///
    /// \param stack the actual stack parameter (the last element in the list
    ///              is treated as the topmost element in the stack.
    ///
    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const = 0;

    ///
    /// \brief create a short description of the instruction
    ///
    /// \return the description of the instruction
    ///
    virtual std::string toString(void) const = 0;
};


///
/// \brief Instruction that loads a variable from the variable stack onto the
///        operation stack.
///
class uetli::code::LoadInstruction : public StackInstruction
{
    /// the index from the top of the variable stack (where 0 is the topmost
    /// element)
    Word fromTop;

public:
    LoadInstruction(Word fromTop);

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief Instruction that stores a value from the operation stack into the
///        variable stack
///
class uetli::code::StoreInstruction : public StackInstruction
{
    /// the index from the top of the variable stack (where 0 is the topmost
    /// element)
    Word fromTop;

public:
    StoreInstruction(Word fromTop);

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief dereferences a pointer
///
/// The value on top of the stack is interpreted as a pointer. It is
/// dereferenced and the new value is put on top of the stack.
///
class uetli::code::DereferenceInstruction : public StackInstruction
{
    Word offset;

public:
    DereferenceInstruction(Word offset);

    virtual void execute(std::vector<void*>& stack, std::vector<void*>&) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief stores a value at a memory location
///
/// The value on top of the stack is popped and stored to the memory location
/// pointed to by the second element on the stack plus an offset.
///
class uetli::code::DereferenceStoreInstruction : public StackInstruction
{
    Word offset;

public:
    DereferenceStoreInstruction(Word offset);

    virtual void execute(std::vector<void*>& stack, std::vector<void*>&) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief pops the top element of the operation stack
///
/// This instruction removes the topmost element of the operation stack
///
class uetli::code::PopInstruction : public StackInstruction
{
public:

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;

    virtual std::string toString(void) const;
};


///
/// \brief calls a subroutine
///
class uetli::code::CallInstruction : public StackInstruction
{
    Subroutine* subroutine;
public:

    CallInstruction(Subroutine* subroutine);

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
    virtual const Subroutine* getSubroutine(void) const;
};


///
/// \brief loads a constant on top of the stack
///
/// The constant is pushed on top of the stack
///
class uetli::code::LoadConstantInstruction : public StackInstruction
{
    Word constant;
public:
    LoadConstantInstruction(Word constant);

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief allocates some memory
///
/// Reads the topmost element on the stack and replaces it with a reference to
/// an allocated memory block, which has the size of the previous value
///
class uetli::code::AllocateInstruction : public StackInstruction
{
public:

    virtual void execute(std::vector<void*>& stack, std::vector<void*>&) const;
    
    virtual std::string toString(void) const;
};


///
/// \brief duplicates the top element
///
/// Reads the element on top of the stack and pushes the same value on the stack
/// a second time.
///
class uetli::code::DuplicateInstruction : public StackInstruction
{
public:

    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
};


class uetli::code::PrintInstruction : public StackInstruction
{
public:
    virtual void execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const;
    
    virtual std::string toString(void) const;
};


class uetli::code::Subroutine
{
protected:
    std::string name;
    size_t argumentCount;
public:
    Subroutine(const std::string& name, size_t argumentCount);
    const std::string& getName(void) const;
    size_t getArgumentCount(void) const;

    virtual ~Subroutine(void);
};


class uetli::code::SubroutineLink : public Subroutine
{
public:
    SubroutineLink(const std::string& name, size_t argumentCount);
};


class uetli::code::DirectSubroutine :
        public Subroutine,
        public StackInstruction
{
protected:
    Word localVariableCount;
    std::vector<StackInstruction*> instructions;

public:
    DirectSubroutine(Word localVariableCount, const std::string& name,
                     size_t argumentCount);

    virtual void execute(std::vector<void*>& stack, std::vector<void*>&) const;

    virtual std::string toString(void) const;

    void addInstruction(StackInstruction* instruction);
    std::vector<StackInstruction*>& getInstructions(void);
    const std::vector<StackInstruction*>& getInstructions(void) const;

    Word getLocalVariableCount(void) const;
    void setLocalVariableCount(Word newCount);
};




#endif // UETLI_CODE_STACKMACHINE_H_

