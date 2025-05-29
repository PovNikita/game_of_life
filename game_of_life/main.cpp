#include <SDL.h>
#include <iostream>
#include <vector>
#include <iostream>
#include "core.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif

const int GRID_SIZE = 20;

int main(int argc, char** argv) {
    using namespace std::literals;
    if(argc != 4)
    {
        return 1;
    }

    Game game(std::stoi(argv[1]), std::stoi(argv[2]), static_cast<uint32_t>(std::stoi(argv[3])));
    game.Run();
    
    return 0;
}
