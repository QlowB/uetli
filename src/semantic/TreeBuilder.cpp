#include "TreeBuilder.h"
#include <iostream>



using namespace uetli::semantic;
using namespace uetli::parser;


TreeBuilder::TreeBuilder(const std::vector<ClassDeclaration*>& declarations) :
    declarations(declarations)
{
}


void TreeBuilder::build(void)
{
    typedef std::vector<ClassDeclaration*>::const_iterator CdIter;
    for (CdIter i = declarations.begin(); i != declarations.end(); i++) {
        EffectiveClass* ec = new EffectiveClass((*i)->name);
        parsedClasses.push_back(ec);
        classesByName.put((*i)->name, ec);
    }

    typedef std::vector<EffectiveClass*>::iterator PcIter;

    for (size_t i = 0; i < declarations.size(); i++) {
        addFeatures(parsedClasses[i], declarations[i]);
    }


    while(!methodsToProcess.empty()) {
        MethodLink ml = methodsToProcess.pop();
        processMethod(ml.second, ml.first);
    }
}


void TreeBuilder::addFeatures(EffectiveClass* effClass, const ClassDeclaration* declaration)
{

    typedef std::vector<FeatureDeclaration*>::const_iterator FdIter;
    typedef std::vector<EffectiveClass*>::iterator PcIter;



    const std::vector<FeatureDeclaration*>& features = declaration->features;

    try {
        for (FdIter i = features.begin(); i != features.end(); i++) {
            FeatureDeclaration* fd = *i;

            EffectiveClass* type = classesByName.get(fd->type);

            if (type == 0) {
                throw "error";
            }

            FieldDeclaration* field = dynamic_cast<FieldDeclaration*> (*i);
            MethodDeclaration* method = dynamic_cast<MethodDeclaration*> (*i);
            if (field != 0) {
                effClass->addField(new Field(effClass, type, field->name));
            }
            else if (method != 0) { // *i is not a field, therefore it must be a method
                Method* m = new Method(effClass, type, method->name);
                effClass->addMethod(m);
                methodsToProcess.push(MethodLink(method, m));
            }
            else {
                throw "internal error";
            }
        }
    }
    catch (const uetli::util::NoEntryException&) {
        std::cerr << "Fatal internal error: Please report this to the developer.\n";
    }
}


void processMethod(Method* method, MethodDeclaration* declaration)
{
    StatementBlock& methodContent = method->getContent();
    std::vector<Statement*>& statements = methodContent.getStatements();


    for (size_t i = 0; i < declaration->body->instructions.size(); i++) {
        Statement* statement = 0;
        //declaration->body->instructions[i];
    }
}


