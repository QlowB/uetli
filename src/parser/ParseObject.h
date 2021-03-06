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

#ifndef UETLI_PARSER_PARSEOBJECT_H_
#define UETLI_PARSER_PARSEOBJECT_H_

#include <string>
#include <vector>
#include "../semantic/AttributedSyntaxTree.h"


namespace uetli
{
    namespace parser
    {
        struct ParseObject;
            struct ClassDeclaration;

            struct FeatureDeclaration;
                struct FieldDeclaration;
                struct MethodDeclaration;

            struct Statement;
                struct NewVariableStatement;
                struct AssignmentStatement;
                struct DoEndBlock;

            struct Expression;
                struct CallOrVariableStatement;
                struct OperationExpression;
                struct BinaryOperationExpression;
                struct UnaryOperationExpression;

            struct ArgumentDeclaration;
    }
}


///
/// \brief base class for every Object generated by
///        the parser
///
/// This structure is the base class for every type of object
/// occurring in the syntax tree of the parsed file.
///
struct uetli::parser::ParseObject
{
};


///
/// \brief A declaration of a class containing the whole definition
///
struct uetli::parser::ClassDeclaration : virtual public ParseObject
{
    std::string name;
    std::vector<FeatureDeclaration*> features;

    ClassDeclaration(const std::string& name,
                     const std::vector<FeatureDeclaration*>& features);

    ~ClassDeclaration(void);

    ///
    /// \brief compares
    /// \return <code>true</code>, if this object depends on the specified, i.e.
    ///         it has a field of the specified type.
    /// \warning at the moment not yet used
    ///
    bool dependsOn(const ClassDeclaration&) const;
};


///
/// \brief base class for an feature in a class (method or field)
///
struct uetli::parser::FeatureDeclaration : virtual public ParseObject
{
    /// \brief the return type of this feature (either its return type, if it's
    ///        a method or the variable type, if it's a field)
    std::string type;
    std::string name;
protected:
    FeatureDeclaration(const std::string& type, const std::string& name);
public:
    virtual ~FeatureDeclaration(void);
};


///
/// \brief Declaration of a field (data variable)
///
struct uetli::parser::FieldDeclaration : virtual public FeatureDeclaration
{
    FieldDeclaration(const std::string& type, const std::string& name);
};


///
/// \brief Declaration of a method
///
struct uetli::parser::MethodDeclaration : virtual public FeatureDeclaration
{
    std::vector<ArgumentDeclaration*> arguments;
    DoEndBlock* body;
    MethodDeclaration(const std::string& type, const std::string& name,
                      DoEndBlock* body);
    ~MethodDeclaration(void);
};


struct uetli::parser::Statement : virtual public ParseObject
{
    virtual ~Statement(void);
    
    virtual semantic::Statement* getAttributedStatement(
            semantic::Scope* scope) const = 0;
};


///
/// \brief declaration of a variable in a scope
///
struct uetli::parser::NewVariableStatement : virtual public Statement
{
    std::string type;
    std::string name;
    Expression* initialValue;

    NewVariableStatement(const std::string& type, const std::string& name);
    NewVariableStatement(const std::string& type, const std::string& name,
                         Expression* initialValue);
    
    ~NewVariableStatement(void);
    
    virtual semantic::Statement* getAttributedStatement(
            semantic::Scope* scope) const;
};


///
/// \brief assignment
///
struct uetli::parser::AssignmentStatement : virtual public Statement
{
    /// left side is a variable statement
    CallOrVariableStatement* leftSide;
    Expression* rightSide;

    AssignmentStatement(CallOrVariableStatement* leftSide,
                        Expression* rightSide);

    virtual semantic::Statement* getAttributedStatement(
            semantic::Scope* scope) const;
};


///
/// \brief collection of instructions between do and end
///
struct uetli::parser::DoEndBlock : virtual public Statement
{
    std::vector<Statement*> statements;

    DoEndBlock(const std::vector<Statement*>& statements);

    virtual semantic::Statement* getAttributedStatement(
            semantic::Scope* scope) const;
};


struct uetli::parser::Expression : virtual public ParseObject
{
    virtual ~Expression(void);

    virtual uetli::semantic::Expression* getAttributedExpression(
            semantic::Scope* scope) const = 0;
};


///
/// \brief an expression representing a function call. This can be an full
///        statement or an expression returning a value. It can also represent
///        a variable identifier.
///
///
/// Since the Uniform Access Principle makes method calls and variable accesses
/// look the same, it is not possible to determine if an expression is a
/// variable access or a function call during the parsing process.
/// This results in this ambiguous data structure which is created by
/// the parser. This finally becomes clear during semantic analysis (building
/// the attributed syntax tree).
///
struct uetli::parser::CallOrVariableStatement :
        virtual public Statement,
        virtual public Expression
{
    /// the name of the called method or the accessed variable
    std::string methodName;

    /// the target expression (e.g. in the case of
    /// <code>var.methodName(5)</code>, <code>var</code> is the target)
    Expression* target;

    /// the arguments which are specified to the call (is always empty for a
    /// variable access)
    std::vector<Expression*> arguments;

    CallOrVariableStatement(Expression* target, const std::string& methodName);
    CallOrVariableStatement(Expression* target, const std::string& methodName,
                  const std::vector<Expression*>& arguments);

    virtual uetli::semantic::Expression* getAttributedExpression(
            semantic::Scope* scope) const;

    virtual uetli::semantic::Statement* getAttributedStatement(
            semantic::Scope* scope) const;
};


struct uetli::parser::OperationExpression : virtual public Expression
{
    std::string operatorToken;

    OperationExpression(const std::string& operatorToken);

    virtual uetli::semantic::Expression* getAttributedExpression(
            semantic::Scope* scope) const = 0;
};


struct uetli::parser::BinaryOperationExpression : public OperationExpression
{
    Expression* left;
    Expression* right;

    BinaryOperationExpression(Expression* left, Expression* right,
                              const std::string& operatorToken);

    virtual uetli::semantic::Expression* getAttributedExpression(
            semantic::Scope* scope) const;
};


struct uetli::parser::UnaryOperationExpression : public OperationExpression
{
    enum Fix {
        PREFIX,
        SUFFIX
    };

    Expression* value;
    Fix fix;

    UnaryOperationExpression(Expression* value, Fix fix,
                             const std::string& operatorToken);

    virtual uetli::semantic::Expression* getAttributedExpression(
            semantic::Scope* scope) const;
};


struct uetli::parser::ArgumentDeclaration
{
    std::string type;
    std::string name;

    ArgumentDeclaration(const std::string& type, const std::string& name);
};


#endif // UETLI_PARSER_PARSEOBJECT_H_




