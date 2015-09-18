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


size_t getNFields(void) const
{
    return fields.size();
}


size_t getNMethods(void) const
{
    return methods.size();
}


Field* getField(size_t index)
{
    return fields[index];
}


Method* getMethod(size_t index)
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


NewVariableStatement(Class* type, const std::string& name) :
	type(type), name(name)
{
}


AssignmentStatement(Variable* lvalue, Expression* rvalue) :
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


Method::Method(Class* returnType, const std::string& name) :
    Feature(returnType, name)
{
}


Scope* Method::getMethodScope(void)
{
    return methodScope;
}


StatementBlock& Method::getContent(void)
{
    return content;
}


