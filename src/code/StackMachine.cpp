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

#include "StackMachine.h"
#include <iostream>
#include <sstream>

using namespace uetli::code;


LoadInstruction::LoadInstruction(Word fromTop) :
    fromTop(fromTop)
{
}


void LoadInstruction::execute(std::vector<void*>& stack,
                              std::vector<void*>& variableStack) const
{
    stack.push_back(variableStack[variableStack.size() - 1 - fromTop]);
}


Word LoadInstruction::getFromTop(void) const
{
    return fromTop;
}


std::string LoadInstruction::toString(void) const
{
    std::stringstream str;
    str << "load " << fromTop << " # load from top of variable stack" <<
        std::endl;    
    return str.str();
}


StoreInstruction::StoreInstruction(Word fromTop) :
    fromTop(fromTop)
{
}


void StoreInstruction::execute(std::vector<void*>& stack,
                               std::vector<void*>& variableStack) const
{
    variableStack[variableStack.size() - 1 - fromTop] = *(stack.end() - 1);
    stack.pop_back();
}


std::string StoreInstruction::toString(void) const
{
    std::stringstream str;
    str << "store " << fromTop << " # store at top of variable stack" <<
        std::endl;    
    return str.str();
}


DereferenceInstruction::DereferenceInstruction(Word offset) :
    offset(offset)
{
}


void DereferenceInstruction::execute(std::vector<void*>& stack,
                                     std::vector<void*>&) const
{
    void* pointer = *(stack.end() - 1);
    pointer = (void*) (((char*) pointer) + offset);
    stack.push_back(*((void**) pointer));
}


std::string DereferenceInstruction::toString(void) const
{
    std::stringstream str;
    str << "dereference " << offset<< " # dereference at top of instruction"
       << " stack (argument = offset)" << std::endl;    
    return str.str();
}


DereferenceStoreInstruction::DereferenceStoreInstruction(Word offset) :
    offset(offset)
{
}


void DereferenceStoreInstruction::execute(std::vector<void*>& stack,
                                          std::vector<void*>&) const
{
    void* pointer = *(stack.end() - 2);
    void* value = *(stack.end() - 1);
    stack.pop_back();
    pointer = (void*) (((char*) pointer) + offset);
    *((void**) pointer) = value;
}


std::string DereferenceStoreInstruction::toString(void) const
{
    std::stringstream str;
    str << "dereference_store " << offset << " # stores the topmost value on "
        << "the operand stack and stores it at the address stored in the second"
       << " value on the operand stack (argument = offset)" << std::endl;
    return str.str();
}


void PopInstruction::execute(std::vector<void*>& stack,
                             std::vector<void*>& variableStack) const
{
    stack.pop_back();
}


std::string PopInstruction::toString(void) const
{
    std::stringstream str;
    str << "pop # pops the topmost value from the operand "
       << "stack" << std::endl;
    return str.str();
}


CallInstruction::CallInstruction(Subroutine* subroutine) :
    subroutine(subroutine)
{
}


void CallInstruction::execute(std::vector<void*>& stack,
                              std::vector<void*>& variableStack) const
{
    DirectSubroutine* ds = dynamic_cast<DirectSubroutine*> (subroutine);
    if (ds != 0) {
        ds->execute(stack, variableStack);
    }
}


std::string CallInstruction::toString(void) const
{
    std::stringstream str;
    str << "call " << subroutine->getName().getAsString() <<
           " # calls a subroutine" <<  std::endl;
    return str.str();
}


const Subroutine* CallInstruction::getSubroutine(void) const
{
    return subroutine;
}


LoadConstantInstruction::LoadConstantInstruction(Word constant) :
    constant(constant)
{
}


void LoadConstantInstruction::execute(std::vector<void*>& stack,
                                      std::vector<void*>& variableStack) const
{
    stack.push_back((void*) constant);
}


std::string LoadConstantInstruction::toString(void) const
{
    std::stringstream str;
    str << "load_const " << constant << " # puts a constant value on the stack"
        << std::endl;
    return str.str();
}



void AllocateInstruction::execute(std::vector<void*>& stack,
                                  std::vector<void*>&) const
{
    Word val = (Word) *(stack.end() - 1);
    stack.pop_back();
    stack.push_back(new char[val]);
}


std::string AllocateInstruction::toString(void) const
{
    std::stringstream str;
    str << "alloc " << " # allocates memory"
        << std::endl;
    return str.str();
}


void DuplicateInstruction::execute(std::vector<void*>& stack,
                                   std::vector<void*>&) const
{
    stack.push_back(*(stack.end() - 1));
}


std::string DuplicateInstruction::toString(void) const
{
    std::stringstream str;
    str << "duplicate # duplicates value on top of stack"
        << std::endl;
    return str.str();
}


void PrintInstruction::execute(std::vector<void*>& stack,
                               std::vector<void*>&) const
{
    std::cout << *(stack.end() - 1) << std::endl;
}


std::string PrintInstruction::toString(void) const
{
    std::stringstream str;
    str << "print # prints out the value on top of the stack"
        << std::endl;
    return str.str();
}


Subroutine::Subroutine(const parser::Identifier& name,
                       size_t argumentCount) :
    name(name),
    argumentCount(argumentCount)
{
}


Subroutine::~Subroutine(void)
{
}


const uetli::parser::Identifier &Subroutine::getName(void) const
{
    return name;
}


size_t Subroutine::getArgumentCount(void) const
{
    return argumentCount;
}


SubroutineLink::SubroutineLink(const parser::Identifier &name, size_t argumentCount) :
    Subroutine(name, argumentCount)
{
}


DirectSubroutine::DirectSubroutine(Word localVariableCount,
                                   const parser::Identifier &name,
                                   size_t argumentCount) :
    Subroutine(name, argumentCount),
    localVariableCount(localVariableCount)
{
}


void DirectSubroutine::execute(std::vector<void*>& stack,
                         std::vector<void*>& variableStack) const
{
    for (Word i = 0; i < localVariableCount; i++)
        variableStack.push_back(0);

    typedef std::vector<StackInstruction*>::const_iterator InstructionIterator;
    for (InstructionIterator i = instructions.begin();
         i != instructions.end(); i++) {
        (*i)->execute(stack, variableStack);
    }

    for (Word i = 0; i < localVariableCount; i++)
        variableStack.pop_back();
}


std::string DirectSubroutine::toString(void) const
{
    std::stringstream str;
    str << "sub " << name.getAsString() << "(" << argumentCount <<
           ") # start of subroutine" << std::endl;

    typedef std::vector<StackInstruction*>::const_iterator InstructionIterator;
    for (InstructionIterator i = instructions.begin();
         i != instructions.end(); i++) {
        str << (*i)->toString();
    }

    str << "end_sub # end of subroutine";

    return str.str();
}


void DirectSubroutine::addInstruction(StackInstruction* instruction)
{
    this->instructions.push_back(instruction);
}


const std::vector<StackInstruction*>&
    DirectSubroutine::getInstructions(void) const
{
    return instructions;
}


std::vector<StackInstruction*>& DirectSubroutine::getInstructions(void)
{
    return instructions;
}


Word DirectSubroutine::getLocalVariableCount(void) const
{
    return localVariableCount;
}


void DirectSubroutine::setLocalVariableCount(Word newCount)
{
    this->localVariableCount = newCount;
}


#include <typeinfo>
namespace uetli { namespace code {
std::string getDescription(uetli::code::StackInstruction* si)
{
    const std::type_info& siInfo = typeid(*si);
    return siInfo.name();
}

}}


