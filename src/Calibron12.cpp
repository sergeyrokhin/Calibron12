#include <algorithm>
#include <numeric>
#include "Solution.h"
#include "render_rectangle.h"
#include "SimpleTimer.h"

void FindSolutions()
{
	RectangleSet rs{ { 21, 14 }, { 21, 14 }, { 28, 14 }, {  4, 14 },
					 { 28,  6 }, { 32, 10 }, { 21, 18 }, { 21, 18 },
					 { 10,  7 }, { 17, 14 }, { 28,  7 }, { 32, 11 } };

	Solutions solutions;

	std::sort(rs.begin(), rs.end());
	std::cout << "The original set afte sort:\n" << rs << std::endl;
	SimpleTimer timer("Calibron12");
	int success_count = 0;
	size_t number_of_attempts = 0;
	do
	{
		auto number_max = 0u;
		for (unsigned variation = 0; variation < END_OF_VARIATION; ++variation) //перебор переворачиванием
		{
			unsigned number = 0;
			++number_of_attempts;
			CalibronBox rect_plasements;
			Placement point = { 0, 0 };
			bool good = true;
			while (NUMBER_OF_RECTANGLE > number)
			{
				if (number_max < number)
				{
					number_max = number;
				}
				NextFreePosition(point, rect_plasements);

				auto [x, y] = GetRectangle(rs, variation, number);
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
					timer.Out(to_string(success_count));
					Print(rect_plasements, variation);
					std::cout << rect_plasements << std::endl;
				}
			}
		}
		//если проверили первые number_max и все они не подходят для начала, то стоит отбросить весь хвост из рассмотрения
		if (++number_max < (NUMBER_OF_RECTANGLE - 1)) //хвост должен быть размером больше 2
		{
			auto start_it = rs.begin();
			std::advance(start_it, number_max);
			std::sort(start_it, rs.end(), [](auto a, auto b) { return a > b; });
		}

	} while (std::next_permutation(rs.begin(), rs.end()));
	std::cout << "Number of attempts: " << number_of_attempts << std::endl;
}

