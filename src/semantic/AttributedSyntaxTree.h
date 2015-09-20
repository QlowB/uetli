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

#ifndef UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_
#define UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_

#include <string>
#include <vector>
#include "../util/HashMap.h"
#include "../code/StackMachine.h"

namespace uetli
{
    namespace semantic
    {
        class Class;
            class ClassReference;
            class EffectiveClass;

        class Scope;
        class Variable;

        class Statement;
            class StatementBlock;
            class NewVariableStatement;
            class AssignmentStatement;
            class CallStatement;
        class Expression;
            class OperationExpression;
            class BinaryOperationExpression;
            class UnaryOperationExpression;

        class Feature;
            class Field;
            class Method;
    }
}


class uetli::semantic::Class
{
    /// \brief defined name of the class
    std::string name;

public:
    Class(const std::string& name);

    ///
    /// \brief the defined name of the class
    /// \return the name of the class
    ///
    virtual const std::string& getName(void) const;
};


class uetli::semantic::ClassReference : public Class
{
public:
    ClassReference(const std::string& name);
};


class uetli::semantic::EffectiveClass : public Class
{
    std::vector<Field*> fields;
    std::vector<Method*> methods;
    uetli::util::HashMap<std::string, Field*> fieldLinks;
    uetli::util::HashMap<std::string, Method*> methodLinks;

public:
    EffectiveClass(const std::string& name);

    void addField(Field* field);
    void addMethod(Method* method);

    size_t getNFields(void) const;
    size_t getNMethods(void) const;

    Field* getField(size_t index);
    Method* getMethod(size_t index);

    Field* getField(const std::string& name);
    Method* getMethod(const std::string& name);
};


class uetli::semantic::Scope
{
    Scope* parentScope;

    uetli::util::HashMap<std::string, Method*> methodLinks;
public:
    Scope(void);

    void setParentScope(Scope* parentScope);

    Method* findMethod(const std::string& name);
};


class uetli::semantic::Variable
{
    Class* type;
    std::string name;
    Scope* scope;
public:
    Variable(Class* type, const std::string& name, Scope* scope);
};


class uetli::semantic::Statement
{
public:
    Statement(void);
    virtual ~Statement(void);

    ///
    /// \brief generate stack machine code and append it to the given vector
    ///
    /// \param code the list to append the code
    ///
    virtual void generateStatementCode(
            std::vector<code::StackInstruction*>& code) const = 0;
};


class uetli::semantic::StatementBlock : public Statement
{
    std::vector<Statement*> statements;
    size_t localVariableCount;

    /// contains the local variables in the order they were defined.
    std::vector<Variable*> localVariables;

    /// maps the local variables to their index in <code>localVariables</code>
    util::HashMap<Variable*, size_t> variableToIndex;
public:
    StatementBlock(void);

    size_t getNStatements(void) const;
    Statement* getStatement(size_t index);
    void addStatement(Statement* toSet);

    size_t getLocalVariableCount(void) const;

    virtual void generateStatementCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::Expression
{
public:
    Expression(void);
    virtual ~Expression(void);

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const = 0;
};


class uetli::semantic::OperationExpression : public Expression
{
    Method* operationMethod;
public:
    OperationExpression(Method* operationMethod);

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const = 0;
};


class uetli::semantic::BinaryOperationExpression : public OperationExpression
{
    Expression* left;
    Expression* right;
public:
    BinaryOperationExpression(Method* operationMethod,
                              Expression* left, Expression* right);

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::UnaryOperationExpression : public OperationExpression
{
    Expression* operand;
public:
    UnaryOperationExpression(Method* operationMethod, Expression* operand);

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::NewVariableStatement : public Statement
{
    Variable newVariable;
public:
    NewVariableStatement(Class* type, const std::string& name, Scope* scope);
    Variable* getVariable(void);

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::AssignmentStatement : public Statement
{
    Variable* lvalue;
    Expression* rvalue;

public:
    ///
    /// \param lvalue destination of the assignment
    /// \param rvalue source of the assignment
    ///
    AssignmentStatement(Variable* lvalue, Expression* rvalue);

    virtual void generateStatementCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::CallStatement : public Statement, public Expression
{
    Method* method;
    std::vector<Expression*> arguments;

public:
    CallStatement(Method* method, const std::vector<Expression*> arguments);

    virtual void generateStatementCode(
            std::vector<code::StackInstruction*>& code) const;

    virtual void generateExpressionCode(
            std::vector<code::StackInstruction*>& code) const;
};


class uetli::semantic::Feature
{
protected:
    Class* wrapper;
    Class* returnType;
    std::string name;
public:
    Feature(Class* wrapper, Class* returnType, const std::string& name);

    Class* getWrapper(void);
    Class* getReturnType(void);
    const std::string& getName(void) const;
};


class uetli::semantic::Field : public Feature
{
public:
    Field(Class* wrapper, Class* type, const std::string& name);
};


class uetli::semantic::Method : public Feature
{
    unsigned int argumentCount;
    Scope methodScope;
    StatementBlock content;
public:
    Method(Class* wrapper, Class* returnType, const std::string& name,
           unsigned int argumentCount);

    Scope* getMethodScope(void);

    StatementBlock& getContent(void);
    const StatementBlock& getContent(void) const;

    unsigned int getArgumentCount(void) const;
};

#endif // UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_

