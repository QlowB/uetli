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


#ifndef UETLI_SEMANTIC_SCOPE_H_
#define UETLI_SEMANTIC_SCOPE_H_

#include "../util/HashMap.h"

#include <string>


namespace uetli
{
    namespace semantic
    {
        class Scope;


        class Method;
        class Class;
        class Variable;
    }
}


///
/// \brief variable and class scope
///
class uetli::semantic::Scope
{
    Scope* parentScope;

    std::vector<Scope*> childrenScopes;

    /// defines if the scope contains a "this" variable
    bool containsThis;

    /// holds variables in the order they were defined
    std::vector<Variable*> variables;

    /// links every variable to an index in the variable list
    /// this map is the reverse function of std::vector<Variable*> variables;
    uetli::util::HashMap<const Variable*, size_t> variableIndices;

    /// holds variables accessible by name
    uetli::util::HashMap<std::string, Variable*> variableLinks;
    uetli::util::HashMap<std::string, Method*> methodLinks;
    uetli::util::HashMap<std::string, Class*> classLinks;
public:
    Scope(void);
    ~Scope(void);

    void setParentScope(Scope* parentScope);

    void setContainsThis(bool containsThis);
    bool getContainsThis(void) const;

protected:
    void addChildScope(Scope* childScope);
public:

    Method* findMethod(const std::string& name);
    Class* findClass(const std::string& name);
    Variable* findVariable(const std::string& name);


    size_t getVariableCount(void) const;

    ///
    /// \brief get the index of a variable from top of the stack
    ///
    /// \param variable the variable to search for
    /// \return the index of this variable if counted from top of the stack
    ///
    size_t getStackIndex(const Variable* variable) const;

    size_t getStackIndexOfThis(void) const;

    void addMethod(Method* variable);
    void addClass(Class* variable);
    void addVariable(Variable* variable);
};


#endif // UETLI_SEMANTIC_SCOPE_H_

