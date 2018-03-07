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

#ifndef UETLI_CODE_STACKCODEGENERATOR_H_
#define UETLI_CODE_STACKCODEGENERATOR_H_

#include "StackMachine.h"
#include "../semantic/AttributedSyntaxTree.h"

namespace uetli
{
    namespace code
    {
        class StackCodeGenerator;
    }
}


class uetli::code::StackCodeGenerator
{
    const semantic::Method* method;
    DirectSubroutine* output;
public:
    StackCodeGenerator(const semantic::Method* method);


    void generateCode(void);
    DirectSubroutine* getGeneratedCode(void);
};



#endif // UETLI_CODE_STACKCODEGENERATOR_H_

