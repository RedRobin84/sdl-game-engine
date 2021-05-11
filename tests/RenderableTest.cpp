#include "Renderable.h"
#include "Registry.h"
#include <vector>
#include <string_view>

//PREPARE
std::vector<std::string_view> vector_of_textures{{ "../assets/img/red.bmp", "../assets/img/green.bmp", "../assets/img/blue.bmp" }};
std::string_view one_texture{"../assets/img/red.bmp"};
Registry registry{};

struct RenderableTest {
    Renderable renderable;
    inline auto getVector() {return renderable.m_textures;}
};

int main()
{

//EXECUTE

    //ONE TEXTURE CTR TEST
    RenderableTest test{Renderable{one_texture, registry}};
    if (test.getVector().size() != 1) {
    printf("RenderableTest:: One texture ctr TEST failed. Vector size not 1");
    return 1;
    }
        //getTexture TEST 1 (not found exception)
        bool exceptionNotCaught{true};
        try {
        auto var = test.renderable.getTexture("not existing record");
        } catch (std::invalid_argument e) {
        exceptionNotCaught = false;
        }
        if (exceptionNotCaught) {
        printf("RenderableTest:: getTexture TEST 1: Expected exception not caught!");
        return 1;
        }
        //getTexture TEST 2 (texture found)
        if (auto texture = test.renderable.getTexture(one_texture); texture == nullptr) {
        printf("RenderableTest:: getTexture TEST 2: Stored texture not retrieved!");
        return 1;
        }
    //MULTI TEXTURE CTR TEST
    RenderableTest test2{Renderable{vector_of_textures, registry}};
     if (test2.getVector().size() != 3) {
    printf("RenderableTest:: Multi texture ctr TEST failed. Vector size not 3");
    return 1;
    }
    return 0;
}
