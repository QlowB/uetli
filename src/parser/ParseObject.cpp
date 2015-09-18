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


NewVariableInstruction::NewVariableInstruction(const std::string& type, const std::string& name) :
    type(type), name(name)
{
}


NewVariableInstruction::NewVariableInstruction(const std::string& type, const std::string& name, Expression* initialValue) :
    type(type), name(name), initialValue(initialValue)
{
}


AssignmentInstruction::AssignmentInstruction(Expression* leftSide, Expression* rightSide) :
	leftSide(leftSide), rightSide(rightSide)
{
}


DoEndBlock::DoEndBlock(const std::vector<Instruction *>& instructions):
    instructions(instructions)
{
}


CallInstruction::CallInstruction(const std::string& methodName) :
    methodName(methodName)
{
}


CallInstruction::CallInstruction(const std::string& methodName, const std::vector<Expression*>& arguments) :
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
