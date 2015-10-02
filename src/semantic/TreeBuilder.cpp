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

#include "TreeBuilder.h"
#include <iostream>
#include <vector>



using namespace uetli;
using namespace uetli::semantic;
using namespace uetli::parser;


TreeBuilder::TreeBuilder(const std::vector<ClassDeclaration*>& declarations) :
    declarations(declarations), globalScope(new Scope())
{
}


TreeBuilder::~TreeBuilder(void)
{
    //delete globalScope;
}


void TreeBuilder::build(void)
{
    typedef std::vector<ClassDeclaration*>::const_iterator CdIter;
    for (CdIter i = declarations.begin(); i != declarations.end(); i++) {

        EffectiveClass* ec = new EffectiveClass((*i)->name);

        ec->getClassScope()->setParentScope(globalScope);

        globalScope->addClass(ec);
        attributedClasses.push_back(ec);
        classesByName.put((*i)->name, ec);
        
    }

    typedef std::vector<EffectiveClass*>::iterator PcIter;

    for (size_t i = 0; i < declarations.size(); i++) {
        addFeatures(attributedClasses[i], declarations[i]);
    }


    while(!methodsToProcess.empty()) {
        MethodLink ml = methodsToProcess.front();
        methodsToProcess.pop();
        processMethod(ml.second, ml.first);
    }
}


const std::vector<uetli::semantic::EffectiveClass*>&
TreeBuilder::getAttributedClasses(void) const
{
    return attributedClasses;
}



void TreeBuilder::addFeatures(EffectiveClass* effClass,
                              const ClassDeclaration* declaration)
{

    typedef std::vector<FeatureDeclaration*>::const_iterator FdIter;
    typedef std::vector<EffectiveClass*>::iterator PcIter;


    const std::vector<FeatureDeclaration*>& features = declaration->features;

    FeatureDeclaration* fd;
    try {
        for (FdIter i = features.begin(); i != features.end(); i++) {
            fd = *i;

            EffectiveClass* type = 0;
            if (!fd->type.empty()) // if return type not "void"
                type = classesByName.get(fd->type);

            FieldDeclaration* field = dynamic_cast<FieldDeclaration*> (*i);
            MethodDeclaration* method = dynamic_cast<MethodDeclaration*> (*i);
            if (field != 0) {
                std::cout << "found field!" << std::endl;
                effClass->addField(new Field(effClass, type, field->name));
            }
            else if (method != 0) {
                std::cout << "found method!" << std::endl;

                // *i is not a field, therefore it must be a method
                Method* m = new Method(effClass, type, method->name,
                                       method->arguments.size());
                effClass->getClassScope()->addMethod(m);
                m->getMethodScope()->setParentScope(effClass->getClassScope());
                //effClass->getClassScope()->addChildScope(m->getMethodScope());
                effClass->addMethod(m);
                methodsToProcess.push(MethodLink(method, m));

                //std::cout << "created attributed entry for method!" <<
                //    std::endl;;
            }
            else {
                throw "internal error";
            }
        }
    }
    catch (const uetli::util::NoEntryException&) {
        std::cerr << "Fatal internal error: " << "Invalid Key: " << fd->type <<
                     ". Please report this to the developer.\n";
    }
}


void TreeBuilder::processMethod(Method* method, MethodDeclaration* declaration)
{
    StatementBlock& methodContent = method->getContent();

    for (size_t i = 0; i < declaration->body->statements.size(); i++) {
        semantic::Statement* statement = declaration->body->statements[i]->
                getAttributedStatement(method->getMethodScope());
        methodContent.addStatement(statement);
    }
}


/// \deprecated
void TreeBuilder::processStatement(EffectiveClass* ec, Scope* scope,
                                   parser::Statement* statement)
{
}



