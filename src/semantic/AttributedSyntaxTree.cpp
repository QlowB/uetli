// ============================================================================
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
// ============================================================================

#include "AttributedSyntaxTree.h"

using namespace uetli::semantic;


Class::Class(const std::string& name) :
    name(name)
{
}


const std::string& Class::getName(void) const
{
    return name;
}


ClassReference::ClassReference(const std::string& name) :
    Class(name)
{
}


EffectiveClass::EffectiveClass(const std::string& name) :
    Class(name)
{
}


void EffectiveClass::addField(Field* field)
{
    fields.push_back(field);
}


void EffectiveClass::addMethod(Method* method)
{
    methods.push_back(method);
}


size_t EffectiveClass::getNFields(void) const
{
    return fields.size();
}


size_t EffectiveClass::getNMethods(void) const
{
    return methods.size();
}


Field* EffectiveClass::getField(size_t index)
{
    return fields[index];
}


Method* EffectiveClass::getMethod(size_t index)
{
    return methods[index];
}


Scope::Scope(void) :
    parentScope(0)
{
}


Variable::Variable(Class* type, const std::string& name, Scope* scope) :
    type(type), name(name), scope(scope)
{
}


Statement::Statement(void)
{
}


Statement::~Statement(void)
{
}


StatementBlock::StatementBlock(void)
{
}


std::vector<Statement*>& StatementBlock::getStatements(void)
{
    return statements;
}


Expression::Expression(void)
{
}


Expression::~Expression(void)
{
}


NewVariableStatement::NewVariableStatement(Class* type, const std::string& name, Scope* scope) :
    newVariable(type, name, scope)
{
}


AssignmentStatement::AssignmentStatement(Variable* lvalue, Expression* rvalue) :
    lvalue(lvalue), rvalue(rvalue)
{
}


Feature::Feature(Class* wrapper, Class* returnType, const std::string& name) :
    wrapper(wrapper), returnType(returnType), name(name)
{
}


Field::Field(Class* wrapper, Class* returnType, const std::string& name) :
    Feature(wrapper, returnType, name)
{
}


Method::Method(Class* wrapper, Class* returnType, const std::string& name) :
    Feature(wrapper, returnType, name)
{
}


Scope* Method::getMethodScope(void)
{
    return &methodScope;
}


StatementBlock& Method::getContent(void)
{
    return content;
}


