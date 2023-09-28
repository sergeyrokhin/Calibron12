#include <algorithm>
#include <numeric>
#include "Solution.h"
#include "render_rectangle.h"

void FindSolutions()
{
	Solutions solutions;

	RectangleSet rs{ { 21, 14 }, { 21, 14 }, { 28, 14 }, {  4, 14 },
					 { 28,  6 }, { 32, 10 }, { 21, 18 }, { 21, 18 },
					 { 10,  7 }, { 17, 14 }, { 28,  7 }, { 32, 11 } };

	std::vector<unsigned> order(rs.size()); //перестановки
	std::iota(order.begin(), order.end(), 0);

	struct Logging {
		unsigned total = 0;
		std::array<unsigned, 4> not_suitable;
	};

	Logging log;
	std::cout << rs << std::endl;
	do
	{
		auto number_max = 0u;
		for (unsigned variation = 0; variation < END_OF_VARIATION; ++variation) //перебор переворачиванием
		{

			++log.total;
			unsigned distance = MAIN_SIDE_WIDTH;
			unsigned back_side = 0;
			unsigned number = 0;
			bool good_consistency = true;
			bool complited_consistency = false;
			//запомнить высоту прямоугольника, поскольку она будет замыкать 4-ю сторону
			auto first_side = GetSideWidth(rs, order, ~variation, number);

			// Проверка, не произойдет ли пересечение с фишками на смежной стороне.
			// с противоположной стороной сравнивать не нужно
			// начинаем сравнение со стороны 1 (0 пропускаем)
			// накапливаем для проверки в rect_plasements
			Calibron12Box rect_plasements;


			for (size_t i = 0; i < 4 && good_consistency; i++, distance = MAIN_SIDE_WIDTH - back_side) //проходим 4 стороны
			{
				if (i == 3) //последняя сторона меньше с обеих сторон. удав укусил себя за хвост
				{
					distance -= first_side;
				}
				while (12 > number)
				{
					if (number_max < number)
					{
						number_max = number;
					}
					//auto last_side = GetSideWidth(rs, order, variation, number);

					auto [x, y] = GetRectangle(rs, order, variation, number);
					//проверка, что фишка гарантированно вписывается
					if (x < 4 || x > distance)
					{
						++log.not_suitable[i];
						//это пока недобор, но будет на следующей перебор
						if (x < 4)
						{
							++number;
						}
						good_consistency = false; //выход из цикла к следующей последовательности
						break;
					}

					//вычисление фишки размещение на столе
					//фишка, первой стороной вдоль края
					Rectangle rect = { x, y };
					auto x0 = MAIN_SIDE_WIDTH - distance;
					if (i == 3) //последняя сторона меньше с обеих сторон. удав укусил себя за хвост
					{
						x0 -= first_side;
					}
					auto new_rect_placement = RectanglePlacement(rect, i, x0);

					for (size_t j = 0; good_consistency && j < i; j++) //не нужно проверять пересечение со стороной i == j
					{
						for (const auto& it : rect_plasements[j]) {
							if (IsIntersect(it, new_rect_placement))
							{
								good_consistency = false; //выход из цикла к следующей последовательности
								break;
							}
						}
					}
					if (!good_consistency)
					{
						break;
					}

					// записываем
					rect_plasements[i].emplace_back(new_rect_placement);

					//это последняя фишка на этой стороне?
					if (x == distance)
					{
						//вычислим загиб на следующую сторону 
						// (инвертируем поразрядное отрицание ~ variation)
						//тогда подсчет будет другой стороны
						back_side = GetSideWidth(rs, order, ~variation, number);
						++number;
						if (i == 3) //не просто закончились фишки, а набрали послеюнюю сторону
						{
							complited_consistency = true;
						}
						break;
					}
					distance -= x;
					++number;
				}
			}
			//подобрали все 4 стороны. нужно вывести результат, добавить в решения
			if (complited_consistency)
			{
				//проверка на слишком узкие свободные места
				unsigned min = GetMinSide(rs, order, number);
				if (!TooTight(rect_plasements, min))
				{
					if (solutions.Add(std::move(Solution(rs, order, variation, number))))
					{
						std::cout << solutions.back() << std::endl;
						Print(rect_plasements, order, variation, rs);

					}
				}
				//А оставшийся хвост можно не вертеть
				//результат будет такой же.
				//оторвать хвост
				variation = TearTail(variation, number);
			}
			else {
				//А оставшийся хвост можно не вертеть
				//результат будет такой же.
				//оторвать хвост
				variation = TearTail(variation, number + 1);
			}
			//или не подобрали.
		}
		//если все переборы не доходили дальше number_max
		//то нет смысла менять местами в хвосте
		//для этого хвост нужно переставить в максимальный порядок, т.е упорядочить по убыванию,
		//чтоб в рассмотрение попала уже новая фишка
		//т.е. следующая перестановка должна будет поменять фишку под номером number_max
		//std::sort(order.rbegin(), order.rend());
		// 
		if (++number_max < 11)
		{
			auto start_it = order.begin();
			std::advance(start_it, number_max);
			std::sort(start_it, order.end(), [](auto a, auto b) { return a > b; });
		}

	} while (std::next_permutation(order.begin(), order.end()));
}

