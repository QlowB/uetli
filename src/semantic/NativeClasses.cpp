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

#include "NativeClasses.h"


using namespace uetli::semantic::native;


Integer::Integer(void) :
    EffectiveClass("Integer")
{
    plus = new Method(this, this, "+", 1);
    minus = new Method(this, this, "-", 1);
    mult = new Method(this, this, "*", 1);
    div = new Method(this, this, "/", 1);

    this->addMethod(plus);
    this->addMethod(minus);
    this->addMethod(mult);
    this->addMethod(div);
}


