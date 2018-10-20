// Subor: main_cli.cpp
// Autor: Daniel Klimaj (xklima22)
//        Vojtěch Večeřa (xvecer18)
// Popis: Main pre CLI verziu hry

#include <string>

#include "boardmodel.h"

int main(int argc, char *argv[])
{
    BoardModel b;

    if(argc == 3)
    {
        if(std::string(argv[1]) == "--load")
        {
            b.load(std::string(argv[2]));
        }
    }
    else if(argc == 1)
    {
        b.createGame();
    }
    else
    {
        return 1;
    }

    return 0;
}
