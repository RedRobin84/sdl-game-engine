#include "Renderable.h"
#include "Registry.h"
#include <vector>
#include <string_view>

std::vector<std::string_view> vector_of_textures{{ "../assets/img/red.bmp", "../assets/img/green.bmp", "../assets/img/blue.bmp" }};

int main()
{
    Registry registry{};

    Renderable renderable{vector_of_textures, registry};
}