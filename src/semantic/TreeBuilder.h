#ifndef UETLI_SEMANTIC_TREEBUILDER_H_
#define UETLI_SEMANTIC_TREEBUILDER_H_

#include "../parser/ParseObject.h"
#include "AttributedSyntaxTree.h"
#include "../util/HashMap.h"

#include <vector>
#include <queue>

namespace uetli
{
    namespace semantic
    {
        class TreeBuilder;
    }
}


class uetli::semantic::TreeBuilder
{

    std::vector<uetli::parser::ClassDeclaration*> declarations;
    std::vector<uetli::semantic::EffectiveClass*> parsedClasses;
    uetli::util::HashMap<std::string, EffectiveClass*> classesByName;

    typedef std::pair<uetli::parser::MethodDeclaration*, Method*> MethodLink;
    /// queue to store methods which will be processed later
    std::queue<MethodLink> methodsToProcess;

public:
    TreeBuilder(const std::vector<uetli::parser::ClassDeclaration*>& declarations);

    void build(void);

private:

    void addFeatures(EffectiveClass* effClass, const uetli::parser::ClassDeclaration* declaration);

    void processMethod(Method* method, uetli::parser::MethodDeclaration* declaration);
};



#endif // UETLI_SEMANTIC_TREEBUILDER_H_

