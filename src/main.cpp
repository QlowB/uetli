#include <iostream>
#include <cstdio>

#include "parser/uetli_parser.h"
#include "semantic/TreeBuilder.h"
#include "code/StackCodeGenerator.h"
#include "code/AssemblyGenerator.h"

using namespace std;


int main(int argc, char** argv)
{
    try {
        cout << "Welcome to Uetli!\n";

        ::uetli_parser_in = stdin;
        ::uetli_parser_parse();

        uetli::semantic::TreeBuilder tb(*parsedClasses);

        tb.build();

        for (size_t i = 0; i < parsedClasses->size(); i++) {
            delete parsedClasses->at(i);
        }
        delete parsedClasses;
        parsedClasses = 0;


        const std::vector<uetli::semantic::EffectiveClass*>& classes =
                tb.getAttributedClasses();

        if (classes.size() > 0) {
            uetli::semantic::EffectiveClass* cl = classes[0];
            std::cout << "Parsed at least one class: " << cl->getName() <<
                         std::endl;
            std::cout << cl->getName() << " has " << cl->getNMethods() <<
                         " methods." << std::endl;
            if (cl->getNMethods() != 0) {
                std::cout << "Parsed at least one method: " <<
                          cl->getMethod(0)->getName() << std::endl;
                uetli::code::StackCodeGenerator scg(cl->getMethod(0));
                scg.generateCode();

                uetli::code::Subroutine* rout = scg.getGeneratedCode();
                for (int i = 0; i < rout->instructions.size(); i++) {
                    std::cout << "instruction: " << uetli::code::getDescription(
                                 rout->instructions[i]) << std::endl;
                }
            }
        }


        std::cout << "Parsed " << classes.size() << " classes\n";

    }
    catch (const char* err) {
        std::cerr << "compilation terminated: " << err << "\n";
    }
    catch (...) {
        std::cerr << "compilation terminated!\n";
    }

    return 0;
}



