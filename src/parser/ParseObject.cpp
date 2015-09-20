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

using namespace uetli::parser;


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


AssignmentStatement::AssignmentStatement(Expression* leftSide,
                                         Expression* rightSide) :
	leftSide(leftSide), rightSide(rightSide)
{
}


DoEndBlock::DoEndBlock(const std::vector<Statement *>& instructions):
    statements(instructions)
{
}


Expression::~Expression(void)
{
}


CallStatement::CallStatement(const std::string& methodName) :
    methodName(methodName)
{
}


CallStatement::CallStatement(const std::string& methodName,
                             const std::vector<Expression*>& arguments) :
    methodName(methodName), arguments(arguments)
{
}


uetli::semantic::Expression* CallStatement::getAttributedExpression(
semantic::Scope* scope) const
{
    semantic::Method* toCall = scope->findMethod(methodName);
    std::vector<semantic::Expression*> arguments;

    if (this->arguments.size() != toCall->getArgumentCount())
        throw "severe internal error!";

    for (unsigned int i = 0; i < toCall->getArgumentCount(); i++) {
        arguments.push_back(this->arguments[i]->getAttributedExpression(scope));
    }

    return new semantic::CallStatement(toCall, arguments);
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
    semantic::Method* operationMethod = scope->findMethod(this->operatorToken);

    semantic::BinaryOperationExpression* boe =
        new semantic::BinaryOperationExpression(operationMethod, left, right);
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
            new semantic::UnaryOperationExpression(operationMethod, operand);

    return uoe;
}


ArgumentDeclaration::ArgumentDeclaration(const std::string& type,
                                         const std::string& name) :
    type(type), name(name)
{
}

/*
#include <stdio.h>
extern FILE* uetli_parser_in;
extern FILE* uetli_parser_out;
#include "Parser.hpp"

extern std::vector<uetli::parser::ClassDeclaration*>* parsedClasses;


int main (void)
{
    ::uetli_parser_in = stdin;
    ::uetli_parser_out = stdout;
    uetli_parser_parse();

    printf("Printing classes!\n");
    if (parsedClasses != 0) {
        printf("Now!\n");
        for (int i = 0; i < parsedClasses->size(); i++) {
            printf("%s\n", parsedClasses->at(i)->name.c_str());
        }
    }
    return 0;
}
*/
