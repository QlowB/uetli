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


#ifndef UETLI_INTERFACE_H_
#define UETLI_INTERFACE_H_

#include <cstdio>
#include <string>
#include <vector>


namespace uetli
{
    class ConsoleInterface;
    
    class UetliConsoleInterface;
}



class uetli::ConsoleInterface
{
protected:
    FILE* in;
    FILE* out;
    FILE* error;


    std::vector<std::string> arguments;

public:

    ConsoleInterface(int argc, char** argv);
    virtual ~ConsoleInterface(void);

    virtual int run(void) = 0;

};


class uetli::UetliConsoleInterface : public ConsoleInterface
{
    std::string outputFilename;
    std::vector<std::string> inputFiles;
    std::vector<FILE*> openedFiles;

    struct Setting
    {
        enum Type
        {
        };

        Type type;
        std::string argument;
    };

    std::vector<Setting> settings;

public:
    UetliConsoleInterface(int argc, char** argv);
    ~UetliConsoleInterface(void);

    virtual int run(void);
};


#endif // UETLI_INTERFACE_H_

