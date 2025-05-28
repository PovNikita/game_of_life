#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <iostream>
#include "core.h"
#include <string>

const int GRID_SIZE = 20;

int main(int argc, char** argv) {
    using namespace std::literals;
    if(argc != 4)
    {
        std::cout << "The number of arguments has to be 3. The current number is: "s << argc - 1 << std::endl;
        return 1;
    }

    Game game(std::stoi(argv[1]), std::stoi(argv[2]), static_cast<uint32_t>(std::stoi(argv[3])));
    game.Run();
    
    return 0;
}
