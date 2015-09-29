/* =============================================================================
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
// ===========================================================================*/


%{

#include <string>
#include <vector>
#include <iostream>
#include "ParseObject.h"
using namespace uetli::parser;

extern int uetli_parser_lex();
FILE* uetli_parser_error_out = stderr;
extern int uetli_parser_error(const char*)
{
    fprintf(::uetli_parser_error_out, "Parser Error!\n");
    return 0;
}

std::vector<uetli::parser::ClassDeclaration*>* parsedClasses = 0;


%}

%define api.prefix {uetli_parser_}

/*
//%skeleton "lalr1.cc" // generate C++ parser
//%define api.namespace {uetli::parser}
//%define api.value.type {struct semantic_type}
//%define parser_class_name {Parser}

//%name-prefix "uetli_parser_"
*/
%union {
    std::vector<uetli::parser::ClassDeclaration*>* classes;
    std::vector<uetli::parser::Statement*>* statements;
    std::vector<uetli::parser::FeatureDeclaration*>* featureList;
    std::vector<uetli::parser::ArgumentDeclaration*>* argumentList;
    std::vector<uetli::parser::Expression*>* expressionList;


    uetli::parser::ClassDeclaration* classDeclaration;

    uetli::parser::FeatureDeclaration* featureDeclaration;
    uetli::parser::FieldDeclaration* fieldDeclaration;
    uetli::parser::MethodDeclaration* methodDeclaration;


    uetli::parser::Statement* statement;
    uetli::parser::NewVariableStatement* newVariableStatement;
    uetli::parser::AssignmentStatement* assignmentStatement;
    uetli::parser::CallOrVariableStatement* callOrVariableStatement;
    uetli::parser::DoEndBlock* doEndBlock;

    uetli::parser::Expression* expression;
    uetli::parser::OperationExpression* operationExpression;
    uetli::parser::BinaryOperationExpression* binaryOperationExpression;
    uetli::parser::UnaryOperationExpression* unaryOperationExpression;

    uetli::parser::ArgumentDeclaration* argumentDeclaration;
    std::string* string;
    const char* cString;

    int token;
};


%token <string> IDENTIFIER
%token <token> CLASS DO END
%token <token> NEW_LINE
%token <token> COLON COMMA ASSIGN OPERATOR
%token <token> ROUND_LEFT ROUND_RIGHT

%type <token> pnl;

%type <classes> classes
%type <statements> statements
%type <featureList> featureList
%type <argumentList> argumentList
%type <expressionList> expressionList

%type <classDeclaration> classDeclaration
%type <featureDeclaration> featureDeclaration
%type <fieldDeclaration> fieldDeclaration
%type <methodDeclaration> methodDeclaration

%type <statement> statement
%type <newVariableStatement> newVariableStatement
%type <assignmentStatement> assignmentStatement
%type <callOrVariableStatement> callOrVariableStatement
%type <doEndBlock> doEndBlock

%type <expression> expression paranthesesExpression
%type <operationExpression> operationExpression
%type <binaryOperationExpression> binaryOperationExpression
%type <unaryOperationExpression> unaryOperationExpression

%type <cString> operator

%type <argumentDeclaration> argumentDeclaration


%left ASTERISK SLASH
%left PLUS MINUS

%start compilationUnit

%%

compilationUnit:
    pnl {
        parsedClasses = new std::vector<ClassDeclaration*>();
    }
    |
    pnl classes pnl {
        parsedClasses = $2;
    };


/* possible newline characters */
pnl:
    /* empty */ {
    }
    |
    pnl NEW_LINE {
    };


/* list of class declarations */
classes:
    classDeclaration {
        $$ = new std::vector<ClassDeclaration*>();
        $$->push_back($1);
    }
    |
    classes pnl classDeclaration {
        $$->push_back($3);
    };


classDeclaration:
    CLASS IDENTIFIER featureList END {
        $$ = new ClassDeclaration(*$2, *$3);
        delete $2; delete $3; $2 = 0; $3 = 0;
    };


featureList:
    pnl {
        $$ = new std::vector<FeatureDeclaration*>();
    }
    |
    featureList featureDeclaration pnl {
        $$ = $1;
        $$->push_back($2);
    };


featureDeclaration:
    fieldDeclaration {
        $$ = $1;
    }
    |
    methodDeclaration {
        $$ = $1;
    };


fieldDeclaration:
    IDENTIFIER COLON IDENTIFIER {
        $$ = new FieldDeclaration(*$3, *$1);
        delete $3; delete $1; $1 = $3 = 0;
    };


methodDeclaration:
    IDENTIFIER COLON IDENTIFIER doEndBlock {
        $$ = new MethodDeclaration(*$3, *$1, $4);
        delete $3; delete $1; $1 = $3 = 0;
    }
    |
    IDENTIFIER doEndBlock {
        $$ = new MethodDeclaration("", *$1, $2);
        delete $1; $1 = 0;
    }
    |
    IDENTIFIER
        ROUND_LEFT argumentList ROUND_RIGHT COLON IDENTIFIER doEndBlock {
        $$ = new MethodDeclaration(*$6, *$1, $7);
        delete $6; delete $1; $1 = $6 = 0;
    }
    |
    IDENTIFIER ROUND_LEFT argumentList ROUND_RIGHT doEndBlock {
        $$ = new MethodDeclaration("", *$1, $5);
        delete $1; $1 = 0;
    };


argumentList:
    argumentDeclaration {
        $$ = new std::vector<ArgumentDeclaration*>();
        $$->push_back($1);
    }
    |
    argumentList COMMA argumentDeclaration {
        $$ = $1;
        $$->push_back($3);
    };


argumentDeclaration:
    IDENTIFIER COLON IDENTIFIER {
        $$ = new ArgumentDeclaration(*$3, *$1);
        delete $3; delete $1; $1 = $3 = 0;
    };


doEndBlock:
    DO statements END {
        $$ = new DoEndBlock(*$2);
        delete $2; $2 = 0;
    };


statements:
    pnl {
        $$ = new std::vector<Statement*>();
    }
    |
    statements statement pnl {
        $$ = $1;
        $$->push_back($2);
    };


statement:
    callOrVariableStatement {
        $$ = $1;
    }
    |
    assignmentStatement {
        $$ = $1;
    }
    |
    newVariableStatement {
        $$ = $1;
    };


callOrVariableStatement:
    IDENTIFIER {
        $$ = new CallOrVariableStatement(*$1);
        delete $1; $1 = 0;
    }
    |
    IDENTIFIER ROUND_LEFT expressionList ROUND_RIGHT {
        $$ = new CallOrVariableStatement(*$1, *$3);
        delete $1; delete $3; $1 = 0; $3 = 0;
    };


/* list of effective arguments */
expressionList:
    expression {
        $$ = new std::vector<Expression*>();
        $$->push_back($1);
    }
    |
    expressionList COMMA expression {
        $$ = $1;
        $$->push_back($3);
    };


expression:
    callOrVariableStatement {
        $$ = $1;
    }
    |
    operationExpression {
        $$ = $1;
    }
    |
    paranthesesExpression {
    	$$ = $1;
    };


operationExpression:
    binaryOperationExpression {
        $$ = $1;
    }
    |
    unaryOperationExpression {
        $$ = $1;
    };


binaryOperationExpression:
    expression operator expression {
        $$ = new BinaryOperationExpression($1, $3, $2);
    };


unaryOperationExpression:
    expression operator {
        $$ = new UnaryOperationExpression($1,
            UnaryOperationExpression::SUFFIX, $2);
    }
    |
    operator expression {
        $$ = new UnaryOperationExpression($2,
            UnaryOperationExpression::PREFIX, $1);
    };


operator:
    PLUS { $$ = "+"; }
    |
    MINUS { $$ = "-"; }
    |
    ASTERISK { $$ = "*"; }
    |
    SLASH { $$ = "/"; };


paranthesesExpression:
    ROUND_LEFT expression ROUND_RIGHT {
        $$ = $2;
    };


assignmentStatement:
    callOrVariableStatement ASSIGN expression {
        $$ = new AssignmentStatement($1, $3);
    };


newVariableStatement:
    IDENTIFIER COLON IDENTIFIER {
        $$ = new NewVariableStatement(*$3, *$1);
        delete $3; delete $1; $3 = 0; $1 = 0;
    };



%%


