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


void PopInstruction::execute(std::vector<void*>& stack,
                             std::vector<void*>& variableStack) const
{
    stack.pop_back();
}


CallInstruction::CallInstruction(Subroutine* subroutine) :
    subroutine(subroutine)
{
}


void CallInstruction::execute(std::vector<void*>& stack,
                              std::vector<void*>& variableStack) const
{
    subroutine->execute(stack, variableStack);
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


void AllocateInstruction::execute(std::vector<void*>& stack,
                                  std::vector<void*>&) const
{
    Word val = (Word) *(stack.end() - 1);
    stack.pop_back();
    stack.push_back(new char[val]);
}


void DuplicateInstruction::execute(std::vector<void*>& stack,
                                   std::vector<void*>&) const
{
    stack.push_back(*(stack.end() - 1));
}


void PrintInstruction::execute(std::vector<void*>& stack,
                               std::vector<void*>&) const
{
    std::cout << *(stack.end() - 1) << std::endl;
}


Subroutine::Subroutine(Word localVariableCount) :
    localVariableCount(localVariableCount)
{
}


void Subroutine::execute(std::vector<void*>& stack,
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


void Subroutine::addInstruction(StackInstruction* instruction)
{
    this->instructions.push_back(instruction);
}


Word Subroutine::getLocalVariableCount(void) const
{
    return localVariableCount;
}


void Subroutine::setLocalVariableCount(Word newCount)
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


