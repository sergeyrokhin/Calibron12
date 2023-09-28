#include <algorithm>
#include <numeric>
#include "Solution.h"
#include "render_rectangle.h"

void FindSolutions()
{
	RectangleSet rs{ { 21, 14 }, { 21, 14 }, { 28, 14 }, {  4, 14 },
					 { 28,  6 }, { 32, 10 }, { 21, 18 }, { 21, 18 },
					 { 10,  7 }, { 17, 14 }, { 28,  7 }, { 32, 11 } };

	std::vector<unsigned> order(rs.size()); //перестановки
	std::iota(order.begin(), order.end(), 0);

	Solutions solutions;

	std::cout << rs << std::endl;
	do
	{
		auto number_max = 0u;
		for (unsigned variation = 0; variation < END_OF_VARIATION; ++variation) //перебор переворачиванием
		{
			unsigned number = 0;
			CalibronBox rect_plasements;
			Placement point = { 0, 0 };
			bool good = true;
			while (12 > number)
			{
				if (number_max < number)
				{
					number_max = number;
				}
				NextFreePosition(point, rect_plasements);

				auto [x, y] = GetRectangle(rs, order, variation, number);
				Rectangle rect = { x, y };
				auto new_rect_placement = RectanglePlacement(rect, point);

				if (!Fit(new_rect_placement, rect_plasements))
				{
					variation = TearTail(variation, number + 1);
					good = false;
					break;
				}
				rect_plasements.emplace_back(new_rect_placement);
				++number;
			}
			if (good)
			{
				if (solutions.Add(rect_plasements))
				{
					Print(rect_plasements, order, variation);
					std::cout << rect_plasements << std::endl;
				}
			}
		}
		if (++number_max < 11)
		{
			auto start_it = order.begin();
			std::advance(start_it, number_max);
			std::sort(start_it, order.end(), [](auto a, auto b) { return a > b; });
		}

	} while (std::next_permutation(order.begin(), order.end()));
}

