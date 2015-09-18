%{

#include <string>
#include <vector>
#include <iostream>
#include "ParseObject.h"
using namespace uetli::parser;

extern int uetli_parser_lex();
extern int uetli_parser_error(const char*)
{
    std::cerr << "Parser Error!\n";
    return 0;
}

std::vector<uetli::parser::ClassDeclaration*>* parsedClasses = 0;


%}


//%skeleton "lalr1.cc" // generate C++ parser
%define api.prefix {uetli_parser_}
//%define api.namespace {uetli::parser}
//%define api.value.type {struct semantic_type}
//%define parser_class_name {Parser}

//%name-prefix "uetli_parser_"

%union {
    std::vector<uetli::parser::ClassDeclaration*>* classes;
    std::vector<uetli::parser::Instruction*>* instructions;
    std::vector<uetli::parser::FeatureDeclaration*>* featureList;
    std::vector<uetli::parser::ArgumentDeclaration*>* argumentList;
    std::vector<uetli::parser::Expression*>* expressionList;


    uetli::parser::ClassDeclaration* classDeclaration;

    uetli::parser::FeatureDeclaration* featureDeclaration;
    uetli::parser::FieldDeclaration* fieldDeclaration;
    uetli::parser::MethodDeclaration* methodDeclaration;


    uetli::parser::Instruction* instruction;
    uetli::parser::AssignmentInstruction* assignmentInstruction;
    uetli::parser::CallInstruction* callInstruction;
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
%type <instructions> instructions
%type <featureList> featureList
%type <argumentList> argumentList
%type <expressionList> expressionList

%type <classDeclaration> classDeclaration
%type <featureDeclaration> featureDeclaration
%type <fieldDeclaration> fieldDeclaration
%type <methodDeclaration> methodDeclaration

%type <instruction> instruction
%type <assignmentInstruction> assignment
%type <callInstruction> callInstruction
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
    IDENTIFIER COLON IDENTIFIER pnl doEndBlock {
        $$ = new MethodDeclaration(*$3, *$1, $5);
        delete $3; delete $1; $1 = $3 = 0;
    }
    |
    IDENTIFIER pnl doEndBlock {
        $$ = new MethodDeclaration("", *$1, $3);
        delete $1; $1 = 0;
    }
    |
    IDENTIFIER ROUND_LEFT argumentList ROUND_RIGHT COLON IDENTIFIER pnl doEndBlock {
        $$ = new MethodDeclaration(*$6, *$1, $8);
        delete $6; delete $1; $1 = $6 = 0;
    }
    |
    IDENTIFIER ROUND_LEFT argumentList ROUND_RIGHT pnl doEndBlock {
        $$ = new MethodDeclaration("", *$1, $6);
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
    DO instructions END {
        $$ = new DoEndBlock(*$2);
        delete $2; $2 = 0;
    };


instructions:
    pnl {
        $$ = new std::vector<Instruction*>();
    }
    |
    instructions instruction pnl {
        $$ = $1;
        $$->push_back($2);
    };


instruction:
    callInstruction {
        $$ = $1;
    }
    |
    assignment {
        $$ = $1;
    };


callInstruction:
    IDENTIFIER {
        $$ = new CallInstruction(*$1);
        delete $1; $1 = 0;
    }
    |
    IDENTIFIER ROUND_LEFT expressionList ROUND_RIGHT {
        $$ = new CallInstruction(*$1, *$3);
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
    callInstruction {
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


assignment:
	expression ASSIGN expression {
		$$ = new AssignmentInstruction($1, $3);
	};



%%


