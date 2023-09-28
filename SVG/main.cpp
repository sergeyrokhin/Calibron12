#include <string_view>
#include <fstream>
#include <iostream>

#include "render_rectangle.h"

#include <filesystem>

const unsigned MAIN_SIDE_WIDTH = 56;

using namespace std::literals;
using namespace std;

int main()
{
    std::cout << "\nHello World!\n";
    {
        std::filesystem::path p = std::filesystem::current_path();
        std::cout << p.string() << std::endl;
    }

    Document doc;
    auto fout = OpenOutputFile("Colibron"s);
    RenderContext out(fout);
    
    AddRectangle(doc, RectanglePlacement{ 0, 32, 0, 10 });
    doc.Render(out);
}

