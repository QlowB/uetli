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


#include "AttributedSyntaxTree.h"
#include "Scope.h"

#include <iostream>

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


Scope* EffectiveClass::getClassScope(void)
{
    return &classScope;
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


Field* EffectiveClass::getField(const std::string& name)
{
    Field** f = fieldLinks.getReference(name);
    return f != 0 ? *f : 0;
}


Method* EffectiveClass::getMethod(const std::string& name)
{
    Method** m = methodLinks.getReference(name);
    return m != 0 ? *m : 0;
}


LanguageObject::LanguageObject(Scope* scope) :
    scope(scope)
{
}


Scope* LanguageObject::getScope(void)
{
    return scope;
}


Statement::Statement(Scope* scope) :
    LanguageObject(scope)
{
}


Statement::~Statement(void)
{
}


StatementBlock::StatementBlock(Scope* scope) :
    LanguageObject(scope),
    Statement(scope)
{
    localScope.setParentScope(scope);
}


size_t StatementBlock::getNStatements(void) const
{
    return statements.size();
}

Statement* StatementBlock::getStatement(size_t index)
{
    return statements[index];
}


void StatementBlock::addStatement(Statement* toSet)
{
    NewVariableStatement* nvs = dynamic_cast<NewVariableStatement*> (toSet);
    if (nvs != 0) {
        localVariableCount++;
        localVariables.push_back(nvs->getVariable());
        variableToIndex.put(nvs->getVariable(), localVariables.size() - 1);
    }
    statements.push_back(toSet);
}


size_t StatementBlock::getLocalVariableCount(void) const
{
    return localVariableCount;
}


Scope* StatementBlock::getLocalScope(void)
{
    return &localScope;
}


#include <typeinfo>
void StatementBlock::generateStatementCode(
        std::vector<code::StackInstruction*>& code) const
{
    typedef std::vector<Statement*>::const_iterator StatIterator;
    std::cout << "transforming: " << statements.size() << " instructions..." <<
                 std::endl;
    for (StatIterator i = statements.begin(); i != statements.end(); i++) {
        std::cout << "transforming an instruction: " << typeid(*i).name() <<
            std::endl;
        (*i)->generateStatementCode(code);
        std::cout << "transformed an instruction." << std::endl;
    }
    std::cout << "transformed " << statements.size() << " instructions." <<
    std::endl;
}


Expression::Expression(Scope* scope) :
    LanguageObject(scope)
{
}


Expression::~Expression(void)
{
}


OperationExpression::OperationExpression(Scope* scope,
                                         Method* operationMethod) :
    LanguageObject(scope),
    Expression(scope), operationMethod(operationMethod)
{
}


BinaryOperationExpression::BinaryOperationExpression(Scope* scope,
                                                     Method* operationMethod,
                                                     Expression* left,
                                                     Expression* right) :
    LanguageObject(scope),
    OperationExpression(scope, operationMethod),
    left(left), right(right)
{
}


void BinaryOperationExpression::generateExpressionCode(
        std::vector<code::StackInstruction*>& code) const
{
    left->generateExpressionCode(code);
    right->generateExpressionCode(code);

    code::CallInstruction* callInstruction = new code::CallInstruction(0);
    code.push_back(callInstruction);
}


UnaryOperationExpression::UnaryOperationExpression(Scope* scope,
                                                   Method* operationMethod,
                                                   Expression* operand) :
    LanguageObject(scope),
    OperationExpression(scope, operationMethod), operand(operand)
{
}


void UnaryOperationExpression::generateExpressionCode(
        std::vector<code::StackInstruction*>& code) const
{
    operand->generateExpressionCode(code);

    code::CallInstruction* callInstruction = new code::CallInstruction(0);
    code.push_back(callInstruction);
}


NewVariableStatement::NewVariableStatement(Scope* scope,
                                           Class* type,
                                           const std::string& name) :
    LanguageObject(scope),
    Statement(scope),
    newVariable(new Variable(scope, type, name))
{
}


NewVariableStatement::~NewVariableStatement(void)
{
    delete newVariable;
    newVariable = 0;
}


Variable* NewVariableStatement::getVariable(void)
{
    return newVariable;
}


void NewVariableStatement::generateStatementCode(
        std::vector<code::StackInstruction*>& code) const
{
}



AssignmentStatement::AssignmentStatement(Scope* scope,
                                         Variable* lvalue,
                                         Expression* rvalue) :
    LanguageObject(scope),
    Statement(scope),
    lvalue(lvalue), rvalue(rvalue)
{
}


void AssignmentStatement::generateStatementCode(
        std::vector<code::StackInstruction*>& code) const
{
    size_t fromTop = scope->getStackIndex(lvalue);
    rvalue->generateExpressionCode(code);
    code::StoreInstruction* store = new code::StoreInstruction(fromTop);
    code.push_back(store);
}


CallStatement::CallStatement(Scope* scope, Method* method,
                             const std::vector<Expression*> arguments) :
    LanguageObject(scope),
    Statement(scope), Expression(scope),
    method(method), arguments(arguments)
{
}


void CallStatement::generateStatementCode(
        std::vector<code::StackInstruction*>& code) const
{
    
    std::cerr << "not yet implemented in file " << __FILE__ << std::endl;
}


void CallStatement::generateExpressionCode(
        std::vector<code::StackInstruction*>& code) const
{
    generateStatementCode(code);
}


Variable::Variable(Scope* scope, Class* type, const std::string& name) :
    LanguageObject(scope),
    Expression(scope),
    type(type), name(name)
{
}


const std::string& Variable::getName(void) const
{
    return name;
}


void Variable::generateExpressionCode(
        std::vector<code::StackInstruction*>& code) const
{
    code.push_back(new code::LoadInstruction(0 /* TODO calc value */ ));
}


Feature::Feature(Class* wrapper, Class* returnType, const std::string& name) :
    wrapper(wrapper), returnType(returnType), name(name)
{
}


Class* Feature::getWrapper(void)
{
    return wrapper;
}


Class* Feature::getReturnType(void)
{
    return returnType;
}


const std::string& Feature::getName(void) const
{
    return name;
}


Field::Field(Class* wrapper, Class* returnType, const std::string& name) :
    Feature(wrapper, returnType, name)
{
}


Method::Method(Class* wrapper, Class* returnType, const std::string& name,
               unsigned int argumentCount) :
    Feature(wrapper, returnType, name),
    argumentCount(argumentCount), content(&methodScope)
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


const StatementBlock& Method::getContent(void) const
{
    return content;
}


unsigned int Method::getArgumentCount(void) const
{
    return argumentCount;
}

