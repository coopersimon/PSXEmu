#include "TestGTE.h"
#include <iostream>

int main(int argc, char **argv)
{
     using namespace test;

     gte device;
     GTEtestbench tester(&device);

     std::cout << "compiled" << std::endl;
}
