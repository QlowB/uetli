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


#include "Scope.h"
#include "AttributedSyntaxTree.h"


using namespace uetli::semantic;


Scope::Scope(void) :
parentScope(0)
{
}


Scope::~Scope(void)
{
    for (std::vector<Scope*>::iterator i = childrenScopes.begin();
         i != childrenScopes.end(); i++) {
        delete (*i);
    }
    childrenScopes.clear();
}


void Scope::setParentScope(Scope* parentScope)
{
    this->parentScope = parentScope;
    parentScope->addChildScope(this);
}


void Scope::addChildScope(Scope* childScope)
{
    this->childrenScopes.push_back(childScope);
}


Method* Scope::findMethod(const std::string& name)
{
    Method** m = methodLinks.getReference(name);
    if (m != 0)
        return *m;
    else if (parentScope != 0) {
        return parentScope->findMethod(name);
    }
    else {
        return 0;
    }
}


Class* Scope::findClass(const std::string& name)
{
    Class** c = classLinks.getReference(name);
    if (c != 0)
        return *c;
    else if (parentScope != 0) {
        return parentScope->findClass(name);
    }
    else {
        return 0;
    }
}


Variable* Scope::findVariable(const std::string& name)
{
    Variable** v = variableLinks.getReference(name);
    if (v != 0)
        return *v;
    else if (parentScope != 0) {
        return parentScope->findVariable(name);
    }
    else {
        return 0;
    }
}


size_t Scope::getVariableCount(void) const
{
    return variables.size();
}


size_t Scope::getVariableIndex(const Variable* variable) const
{
    return getVariableCount() - variableIndices.get(variable);
}
 

void Scope::addMethod(Method* method)
{
    methodLinks.put(method->getName(), method);
}


void Scope::addClass(Class* newClass)
{
    classLinks.put(newClass->getName(),newClass);
}


void Scope::addVariable(Variable* variable)
{
    variables.push_back(variable);
    variableIndices.put(variable, variables.size() - 1);
    variableLinks.put(variable->getName(), variable);
}


