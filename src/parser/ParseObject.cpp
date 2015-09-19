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

#include "ParseObject.h"

using namespace uetli::parser;


ClassDeclaration::ClassDeclaration(const std::string& name, const std::vector<FeatureDeclaration *>& features) :
    name(name), features(features)
{
}


bool ClassDeclaration::dependsOn(const ClassDeclaration&) const
{
    return false;
}


FeatureDeclaration::FeatureDeclaration(const std::string& type, const std::string& name) :
    type(type), name(name)
{
}


FeatureDeclaration::~FeatureDeclaration(void)
{
}


FieldDeclaration::FieldDeclaration(const std::string& type, const std::string& name) :
    FeatureDeclaration(type, name)
{
}


MethodDeclaration::MethodDeclaration(const std::string& type, const std::string& name, DoEndBlock* body) :
    FeatureDeclaration(type, name), body(body)
{
}


NewVariableStatement::NewVariableStatement(const std::string& type, const std::string& name) :
    type(type), name(name)
{
}


NewVariableStatement::NewVariableStatement(const std::string& type, const std::string& name, Expression* initialValue) :
    type(type), name(name), initialValue(initialValue)
{
}


AssignmentStatement::AssignmentStatement(Expression* leftSide, Expression* rightSide) :
	leftSide(leftSide), rightSide(rightSide)
{
}


DoEndBlock::DoEndBlock(const std::vector<Statement *>& instructions):
    statements(instructions)
{
}


CallStatement::CallStatement(const std::string& methodName) :
    methodName(methodName)
{
}


CallStatement::CallStatement(const std::string& methodName, const std::vector<Expression*>& arguments) :
    methodName(methodName), arguments(arguments)
{
}


OperationExpression::OperationExpression(const std::string& operatorToken) :
    operatorToken(operatorToken)
{
}


BinaryOperationExpression::BinaryOperationExpression(Expression* left, Expression* right, const std::string& operatorToken) :
    OperationExpression(operatorToken), left(left), right(right)
{
}


UnaryOperationExpression::UnaryOperationExpression(Expression* value, Fix fix, const std::string& operatorToken) :
    OperationExpression(operatorToken), value(value), fix(fix)
{
}


ArgumentDeclaration::ArgumentDeclaration(const std::string& type, const std::string& name) :
    type(type), name(name)
{
}


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
