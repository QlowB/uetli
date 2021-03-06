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

#include "ParseObject.h"
#include <iostream>

using namespace uetli::parser;
using namespace uetli;


ClassDeclaration::ClassDeclaration(
        const std::string& name,
        const std::vector<FeatureDeclaration*>& features) :
    name(name), features(features)
{
}


ClassDeclaration::~ClassDeclaration(void)
{
    typedef std::vector<FeatureDeclaration*>::iterator FeatIterator;

    for (FeatIterator i = features.begin(); i != features.end(); i++) {
        delete *i;
    }
    features.clear();
}


bool ClassDeclaration::dependsOn(const ClassDeclaration&) const
{
    return false;
}


FeatureDeclaration::FeatureDeclaration(const std::string& type,
                                       const std::string& name) :
    type(type), name(name)
{
}


FeatureDeclaration::~FeatureDeclaration(void)
{
}


FieldDeclaration::FieldDeclaration(const std::string& type,
                                   const std::string& name) :
    FeatureDeclaration(type, name)
{
}


MethodDeclaration::MethodDeclaration(const std::string& type,
                                     const std::string& name,
                                     DoEndBlock* body) :
    FeatureDeclaration(type, name), body(body)
{
}


MethodDeclaration::~MethodDeclaration(void)
{
    typedef std::vector<ArgumentDeclaration*>::iterator ArgIterator;
    for (ArgIterator i = arguments.begin(); i != arguments.end(); i++) {
        delete *i;
    }
    arguments.clear();
}


Statement::~Statement(void)
{
}


NewVariableStatement::NewVariableStatement(const std::string& type,
                                           const std::string& name) :
    type(type), name(name)
{
}


NewVariableStatement::NewVariableStatement(const std::string& type,
                                           const std::string& name,
                                           Expression* initialValue) :
    type(type), name(name), initialValue(initialValue)
{
}


NewVariableStatement::~NewVariableStatement(void)
{
    if (initialValue != 0) {
        delete initialValue;
        initialValue = 0;
    }
}


uetli::semantic::Statement* NewVariableStatement::getAttributedStatement(
        semantic::Scope* scope) const
{
    semantic::NewVariableStatement* nvs =
        new semantic::NewVariableStatement(scope, scope->findClass(type), name);
    scope->addVariable(nvs->getVariable());
    return nvs;
}


AssignmentStatement::AssignmentStatement(CallOrVariableStatement* leftSide,
                                         Expression* rightSide) :
	leftSide(leftSide), rightSide(rightSide)
{
}


semantic::Statement* AssignmentStatement::getAttributedStatement(
        semantic::Scope* scope) const
{
    semantic::Expression* left = leftSide->getAttributedExpression(scope);
    semantic::Variable* lvalue = dynamic_cast<semantic::Variable*>(left);
    if (lvalue == 0)
        throw "lvalue must be variable";

    return new semantic::AssignmentStatement(
        scope,
        lvalue,
        rightSide->getAttributedExpression(scope)
    );
}


DoEndBlock::DoEndBlock(const std::vector<Statement *>& instructions):
    statements(instructions)
{
}


semantic::Statement* DoEndBlock::getAttributedStatement(
        semantic::Scope* scope) const
{
    semantic::StatementBlock* block = new semantic::StatementBlock(scope);

    typedef std::vector<Statement*>::const_iterator StatIterator;
    for (StatIterator i = statements.begin(); i != statements.end(); i++) {
        block->addStatement(
                (*i)->getAttributedStatement(block->getLocalScope())
        );
    }

    return block;
}


Expression::~Expression(void)
{
}


CallOrVariableStatement::CallOrVariableStatement
    (Expression* target, const std::string& methodName) :
    methodName(methodName), target(target)
{
}


CallOrVariableStatement::CallOrVariableStatement(
        Expression* target,
        const std::string& methodName,
        const std::vector<Expression*>& arguments) :
    methodName(methodName), target(target), arguments(arguments)
{
}


uetli::semantic::Expression* CallOrVariableStatement::getAttributedExpression(
        semantic::Scope* scope) const
{
    //std::cout << "searching for method: " << methodName << std::endl;
    semantic::Method* toCall = scope->findMethod(methodName);

    if (toCall != 0) {
        std::vector<semantic::Expression*> arguments;

        if (this->arguments.size() != toCall->getArgumentCount())
            throw "severe internal error: method has wrong argument count.";

        for (unsigned int i = 0; i < toCall->getArgumentCount(); i++) {
            arguments.push_back(
                        this->arguments[i]->getAttributedExpression(scope));
        }

        semantic::Expression* target = 0;

        if (this->target != 0) {
            target = this->target->getAttributedExpression(scope);
        }

        return new semantic::CallStatement(scope, target, toCall, arguments);
    }

    semantic::Variable* toExpress = scope->findVariable(methodName);
    if (toExpress != 0/* && arguments.empty() */) {
        return toExpress;
    }
    else
        throw "severe internal error: feature is neither method nor field.";
}


uetli::semantic::Statement* CallOrVariableStatement::getAttributedStatement(
        semantic::Scope* scope) const
{
    return dynamic_cast<semantic::Statement*> (getAttributedExpression(scope));
}


OperationExpression::OperationExpression(const std::string& operatorToken) :
    operatorToken(operatorToken)
{
}


BinaryOperationExpression::BinaryOperationExpression(
        Expression* left, Expression* right,
        const std::string& operatorToken) :
    OperationExpression(operatorToken), left(left), right(right)
{
}


uetli::semantic::Expression* BinaryOperationExpression::getAttributedExpression(
        semantic::Scope* scope) const
{
    semantic::Expression* left = this->left->getAttributedExpression(scope);
    semantic::Expression* right = this->right->getAttributedExpression(scope);

    semantic::EffectiveClass* leftType = dynamic_cast<semantic::EffectiveClass*>
        (left->getStaticType());

    semantic::Method* operationMethod = leftType->getClassScope()
        ->findMethod(this->operatorToken);

    if (operationMethod == 0) {
        std::cerr << "operation " << this->operatorToken <<
            " not defined" << std::endl;
        throw "undefined operator!";
//        operationMethod = new semantic::Method(0, 0,
//                this->operatorToken, 2);
    }


    semantic::BinaryOperationExpression* boe =
        new semantic::BinaryOperationExpression(scope, operationMethod,
                                                left, right);
    return boe;
}


UnaryOperationExpression::UnaryOperationExpression(
        Expression* value,
        Fix fix,
        const std::string& operatorToken) :
    OperationExpression(operatorToken), value(value), fix(fix)
{
}


uetli::semantic::Expression* UnaryOperationExpression::getAttributedExpression(
        semantic::Scope* scope) const
{
    semantic::Expression* operand = this->value->getAttributedExpression(scope);
    semantic::Method* operationMethod = scope->findMethod(this->operatorToken);

    if (operationMethod == 0)
        throw "error: no such unary operator defined!";

    semantic::UnaryOperationExpression* uoe =
            new semantic::UnaryOperationExpression(scope, operationMethod,
                                                   operand);

    return uoe;
}


ArgumentDeclaration::ArgumentDeclaration(const std::string& type,
                                         const std::string& name) :
    type(type), name(name)
{
}

