#include <iostream>
#include "shell.h"


using namespace std;

int main(int argc, char *argv[], char ** env)
{
    auto shell = new Shell(env);

    shell->run();

    return 0;
}