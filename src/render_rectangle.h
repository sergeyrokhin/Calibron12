#pragma once
#include "svg.h"
#include "rectangle.h"


using namespace svg;
using namespace std;

extern const unsigned PIXEL_PER_POINT;

ofstream OpenOutputFile(const string& file_name);

void AddRectangle(Document& doc, const RectanglePlacement& rect);

void Print(const CalibronBox& pl_set, unsigned& variation);