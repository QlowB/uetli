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


///
/// \brief The uetli::semantic::TreeBuilder class
///
/// \author Nicolas Winkler
///
class uetli::semantic::TreeBuilder
{
    /// list of classes received from the parser
    std::vector<uetli::parser::ClassDeclaration*> declarations;

    /// list which contains the newly attributed classes
    std::vector<uetli::semantic::EffectiveClass*> attributedClasses;

    /// this map links the name of a class to the corresponding
    /// attributed class
    uetli::util::HashMap<std::string, EffectiveClass*> classesByName;

    /// pair linking the freshly parsed MethodDeclaration and an attributed
    /// Method
    typedef std::pair<uetli::parser::MethodDeclaration*, Method*> MethodLink;

    /// queue to store methods which will be processed later
    std::queue<MethodLink> methodsToProcess;

public:
    ///
    /// \brief initialize the builder with a list of parsed classes
    /// \param declarations the list of parsed classes
    ///
    TreeBuilder(
            const std::vector<uetli::parser::ClassDeclaration*>& declarations);

    ///
    /// \brief build the attributed tree
    ///
    void build(void);

    ///
    /// \brief get the attributed classes
    /// \return the processed (attributed) class trees
    ///
    const std::vector<uetli::semantic::EffectiveClass*>&
    getAttributedClasses(void) const;

private:

    void addFeatures(EffectiveClass* effClass,
                     const uetli::parser::ClassDeclaration* declaration);

    void processMethod(Method* method,
                       uetli::parser::MethodDeclaration* declaration);

    void processStatement(EffectiveClass* ec, Scope* scope,
                          uetli::parser::Statement*statement);
};



#endif // UETLI_SEMANTIC_TREEBUILDER_H_

