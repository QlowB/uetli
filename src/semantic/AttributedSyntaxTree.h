#ifndef UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_
#define UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_

#include <string>
#include <vector>

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
        class Expression;

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

public:
    EffectiveClass(const std::string& name);

    void addField(Field* field);
    void addMethod(Method* method);

    size_t getNFields(void) const;
    size_t getNMethods(void) const;

    Field* getField(size_t index);
    Method* getMethod(size_t index);
};


class uetli::semantic::Scope
{
    Scope* parentScope;
public:
	Scope(void);

    void setParentScope(Scope* parentScope);
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
};


class uetli::semantic::StatementBlock : public Statement
{
    std::vector<Statement*> statements;
public:
    StatementBlock(void);

    std::vector<Statement*>& getStatements(void);
};


class uetli::semantic::Expression
{
public:
    Expression(void);
    virtual ~Expression(void);
};


class uetli::semantic::NewVariableStatement : public Statement
{
	Variable newVariable;
public:
	NewVariableStatement(Class* type, const std::string& name);
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
};


class uetli::semantic::Feature
{
protected:
    Class* wrapper;
    Class* returnType;
    std::string name;
public:
    Feature(Class* wrapper, Class* returnType, const std::string& name);
};


class uetli::semantic::Field : public Feature
{
public:
    Field(Class* wrapper, Class* type, const std::string& name);
};


class uetli::semantic::Method : public Feature
{
    Scope methodScope;
    StatementBlock content;
public:
    Method(Class* returnType, const std::string& name);

    Scope* getMethodScope(void);

    StatementBlock& getContent(void);
};

#endif // UETLI_SEMANTIC_ATTRIBUTEDSYNTAXTREE_H_

