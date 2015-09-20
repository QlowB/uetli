#include <iostream>
#include "StackMachine.h"

using namespace std;
using namespace uetli::code;

int main(int argc, char** argv)
{
    std::vector<void*> stack;
    std::vector<void*> variableStack;
    stack.push_back((void*) new void*((void*) 0xDEADBEEF));

    Subroutine sub(5);

    sub.instructions.push_back(new DereferenceInstruction(0));
    sub.instructions.push_back(new PrintInstruction());

    sub.execute(stack, variableStack);

}
