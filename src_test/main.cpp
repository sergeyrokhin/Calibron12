#include "rectangle_test.h"

const RectangleSet rs{ { 21, 14 }, { 21, 14 }, { 28, 14 }, {  4, 14 },
				 { 28,  6 }, { 32, 10 }, { 21, 18 }, { 21, 18 },
				 { 10,  7 }, { 17, 14 }, { 28,  7 }, { 32, 11 } };
int main() {
	TestRectangleSet(rs);
	TestIsIntersect();
	return 0;
}