#include <fstream>
#include "render_rectangle.h"


const coordinate PIXEL_PER_POINT = 5;

//svg::Color color1;                               // none
//svg::Color color2 = svg::Rgb{ 215, 30, 25 };       // rgb(215,30,25)
//svg::Color color3 = svg::NoneColor;              // none
//svg::Color color4 = svg::Rgba{ 15, 15, 25, 0.7 };  // rgba(15,15,25,0.7)
//svg::Color color5 = "red"s;                      // red
//svg::Color color6 = svg::Rgb{};                  // rgb(0,0,0)
//svg::Color color7 = svg::Rgba{};                 // rgba(0,0,0,1.0); 

ofstream OpenOutputFile(const string& file_name) {
    string filename = file_name + ".svg";
    ofstream out(filename);
    if (!(out))
    {
        cout << "Error open output file: " << filename << " \n"s;
    }
    return out;
}

void AddRectangle(Document& doc, const RectanglePlacement& rect)
{
    Polyline polyline;
    static svg::Color black_color = svg::Rgb{ 0, 0, 0 };

    auto dx = rect.x2 - rect.x1;
    auto dy = rect.y2 - rect.y1;
    if (dx < dy)
    {
        auto temp = dx;
        dx = dy;
        dy = temp;
    }
    uint8_t red = dx * 8, green = dy * 8, blue = (dx + dy) * 4;
    svg::Color color2 = svg::Rgba{ red, green, blue, 0.5 };

    coordinate p_x1 = rect.x1 * PIXEL_PER_POINT;
    coordinate p_x2 = rect.x2 * PIXEL_PER_POINT;
    coordinate p_y1 = rect.y1 * PIXEL_PER_POINT;
    coordinate p_y2 = rect.y2 * PIXEL_PER_POINT;

    polyline.AddPoint({ p_x1, p_y1 });
    polyline.AddPoint({ p_x2, p_y1 });
    polyline.AddPoint({ p_x2, p_y2 });
    polyline.AddPoint({ p_x1, p_y2 });
    polyline.AddPoint({ p_x1, p_y1 });

    polyline.SetStrokeWidth(PIXEL_PER_POINT / 2);
    polyline.SetFillColor(color2);
    polyline.SetStrokeColor(black_color);
    doc.Add(move(polyline));

    string name = to_string(dx) + "x"s + to_string(dy);
    doc.Add(Text().SetPosition({ p_x1 + PIXEL_PER_POINT, p_y1 + PIXEL_PER_POINT })
        .SetFontFamily("Verdana"s)
        .SetFontSize(12)
        .SetFillColor("white"s)
        .SetOffset({ 0, 6 })
        .SetFontWeight("bold"s)
        .SetData(name));

}

void Print(const Calibron12Box& pl_set, std::vector<unsigned>& order, unsigned& variation, const RectangleSet& rs)
{
    string name_file(order.size(), ' ');
    name_file.reserve(20);
    for (size_t i = 0; i < order.size(); i++)
    {
        name_file[i] = 'A' + order[i];
    }
    name_file = "svg/"s + name_file + to_string(variation);
    Document doc;
    auto fout = OpenOutputFile(name_file);
    RenderContext out(fout);
    size_t l = 0;
    for (size_t i = 0; i < 4; i++)
    {
        for (const auto j : pl_set[i])
        {
            AddRectangle(doc, j);
            ++l;
        }
    }
    Placement pl = { 0 , MAIN_SIDE_WIDTH + 5};

    for (size_t i = l; i < rs.size(); i++)
    {
        AddRectangle(doc, RectanglePlacement(rs [order[i]], pl));
        pl[0] += 33;
    }

    doc.Render(out);

}
