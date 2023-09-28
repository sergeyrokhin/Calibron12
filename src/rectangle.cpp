#include "rectangle.h"
#include "rectangle.h"

const unsigned MAIN_SIDE_WIDTH = 56;
const unsigned END_OF_VARIATION = (1 << 12); // 1 0000 0000 0000 в двоичной

std::ostream& operator<<(std::ostream& out, const Rectangle& c) {
	out << '[' << c[0] << "x" << c[1] << ']';
	return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& a) {
	out << '{';
	if (a.size() > 0)
	{
		auto it = a.cbegin();
		out << *it;
		while (++it != a.cend())
		{
			out << ",    " << *it;
		}
	}
	out << '}';
	return out;
}

RectanglePlacement::RectanglePlacement(Rectangle rect, unsigned side_number, unsigned x0)
{
	switch (side_number)
	{
	case 0:
		x1 = x0;
		y1 = 0;

		x2 = x0 + rect[0];
		y2 = rect[1];

		break;
	case 1:
		x1 = MAIN_SIDE_WIDTH - rect[1];
		y1 = x0;

		x2 = MAIN_SIDE_WIDTH;
		y2 = x0 + rect[0];
		break;
	case 2:
		x1 = MAIN_SIDE_WIDTH - x0 - rect[0];
		y1 = MAIN_SIDE_WIDTH - rect[1];

		x2 = MAIN_SIDE_WIDTH - x0;
		y2 = MAIN_SIDE_WIDTH;
		break;
	case 3:
		x1 = 0;
		y1 = MAIN_SIDE_WIDTH - x0 - rect[0];
		if (x0 + rect[0] > MAIN_SIDE_WIDTH)
		{
			auto y = MAIN_SIDE_WIDTH - x0 - rect[0];
		}
		x2 = rect[1];
		y2 = MAIN_SIDE_WIDTH - x0;
		break;
	default:
		assert(false);
		break;
	}
}


unsigned GetNumberSide(unsigned variation, unsigned number) {
	number = 12 - 1 - number;
	auto side_number = (variation >> number) % 2;
	return side_number;
}

unsigned TearTail(unsigned variation, unsigned number) {
	//Перевороты последних разрядов не требуется
	//старшие разряды (number) не трогаем, в хвосте нарисуем (12 - number)
	if (11 < number)
	{
		return variation;
	}
	unsigned variation_add = 1;
	auto number_bit = 12 - number;
	for (size_t i = 0; i < number_bit; i++)
	{
		variation |= variation_add; //побитовое ИЛИ
		variation_add <<= 1; //побитово сдвинуть на один разряд
	}
	return variation;
}


unsigned GetSideWidth(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned variation, unsigned number) {
	return rs[order[number]][GetNumberSide(variation, number)];
}

bool Inside(const Placement &pl, const RectanglePlacement &rect_pl) {
	return (rect_pl.x1 <= pl[0] && pl[0] < rect_pl.x2 && rect_pl.y1 <= pl[1] && pl[1] < rect_pl.y2); //точка внутри прямоугольника, не на границе
}

bool PlacementIsBusy(const Placement &pl, const Calibron12Box& cb) {
	for (const auto& i : cb) //по каждой стороне
	{
		for (const auto& j : i) // каждый прямоугольник
		{
			if (Inside(pl, j))
			{
				return  true; //точка внутри прямоугольника, не на границе
			}
		}
	}
	return false;
}


//возвращает Истина, если 
//указывает на свободную точку
//если точка занята, то сдвигает до свободной или за край, возвращая ложь
bool FirstFree(Placement& pl, bool vertically, const Calibron12Box& rect_plasements) {
	//++pl[vertically];//вероятно стояли на занятой, двигаем на следующую позицию
	bool search_has_changed = true;
	//определили точку, нужно её проверить. Это начало, 
	// сюда следует вернуться, если точку сдвинем
	while (search_has_changed && pl[vertically] < MAIN_SIDE_WIDTH)
	{
		search_has_changed = false; //если окажется свободной, то выйдем из цикла
		for (int i = 0; i < 4 && !search_has_changed && pl[vertically] < MAIN_SIDE_WIDTH; i++) //по каждой стороне
		{
			for (const auto& j : rect_plasements[i]) // каждый прямоугольник
			{
				if (Inside(pl, j)) //точка внутри прямоугольника, не на границе
				{
					//передвигаемся за прямоугольник
					pl[vertically] = (vertically ? j.y2 : j.x2); //на следующую после этого прямоугольника
					//повтор цикла по i
					i = - 1;
					search_has_changed = true;
					//не свободный, передвинулись, значит нужно снова вернуться на
					// начало цикла while 
					break;
				}
			}
		}
	}
	return pl[vertically] < MAIN_SIDE_WIDTH;
}

//возвращает Истина, если 
//указывает на ближайшую несвободную точку
// но инвариант - края заняты, мы не выйдем за границу, 
//если точка свободна, то сдвигает на следующую, возвращая ложь
bool LastFree(Placement& pl, bool vertically, const Calibron12Box& rect_plasements) {
	//	++pl[vertically];//вероятно стояли на свободной, двигаем на следующую позицию
	while (pl[vertically] < MAIN_SIDE_WIDTH)
	{
		for (size_t i = 0; i < 4; i++) //по каждой стороне, пока не сменили точку проверки
		{
			for (const auto& j : rect_plasements[i]) // каждый прямоугольник
			{
				if (Inside(pl, j)) //точка внутри прямоугольника, не на границе
				{
					//наткнулись на прямоугольник
					return true;
				}
			}
		}
		//следующую точку проверяем
		++pl[vertically];
	}
	//поскольку по краям все должно быть занято, находясь внутри
	//невозможно выйти за пределы поля.
	assert(false);
	return false; //нашли
}

unsigned GetMinSide(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned number)
{
	unsigned min = 18; //21x18 это максимальный размер для минимальной, но можно и 56 записать, заведомо будет меньше
	for (size_t i = number; i < 12; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (rs[order[i]][j] < min)
			{
				min = rs[order[i]][j];
			}
		}
	}
	return min;
}

// Эту щель никто не закроет
bool TooTight(const Calibron12Box& rect_plasements, unsigned min)
{
	// все стенки заняты, свободной может быть только дальше от стены на 4
	for (unsigned d = 3; d < MAIN_SIDE_WIDTH - 4; d++) //проверяем каждую
	{
		//vertically == 0 - проверяем строки, перебирая каждый слой по оси "y"
		for (size_t vertically = 0; vertically < 2; vertically++) //горизонталь и вертикаль
		{
			Placement point = { 3, 3 };
			//
			point[(vertically + 1) % 2] = d;
			while (FirstFree(point, vertically, rect_plasements))
			{
				unsigned start_free = point[vertically];
				LastFree(point, vertically, rect_plasements);
				if (min > point[vertically] - start_free)
				{
					return true;
				}
			}
		}
	}
	return false;
}


std::pair<unsigned, unsigned> GetRectangle(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned variation, unsigned number)
{
	auto& rect = rs[order[number]];
	auto side_num = GetNumberSide(variation, number);
	return { rect[side_num], rect[(side_num + 1) % 2] };
}

template <typename T>
inline bool between(T x, T x1, T x2) {
	if (x > x1 && x < x2)
	{
		return true;
	}
	return false;
}
// ИНВАРИАНТ для RectanglePlacement 
// x1 < x2, y1 < y2;
bool IsIntersectAB(const RectanglePlacement& a, const RectanglePlacement& b)
{
	// a правее ИЛИ левее
	if (a.x1 >= b.x2 || a.x2 <= b.x1)
	{
		return false;
	}
	// значит либо левый, либо правый угол a между b.x1 или b.x2
	// и если хоть одна из горизонталей a находится между горизонталями b, значит есть пересечение
	if (between(a.y1, b.y1, b.y2))
	{
		return true;
	}
	if (between(a.y2, b.y1, b.y2))
	{
		return true;
	}
	return false;
}


bool IsIntersect(const RectanglePlacement& a, const RectanglePlacement& b)
{
	// в нашем случае не может быть, когда одна полностью накрывает другую
	// т.е. пересечение сторон необходимо и достаточно для несовместимости
	// будем искать пересечения сторон
	// Достаточно найти пересечение либо по вертикали, либо по горизонтали
	// Если есть пересечение, то либо a пересекает b по горизонтали, либо b пересекает a
	// Проверяем только вертикальное пересечение, a -> b потом b -> a

	return (IsIntersectAB(a, b) || IsIntersectAB(b, a));
}

namespace
{
	//процедуры попытки найти место для каждого прямоугольника
	//для этого находим все пустые углы
	//каждый должен быть занят хотя бы один раз
	//каждому прямоугольнику, которые в хвосте (от number до 12) нужно сопоставить хоть один угол
	struct Corner {
		Placement pl;

	};


	bool IsCorrectCorners(const Calibron12Box & rect_plasements, const RectangleSet & rs, unsigned number) {
		return true;
	}
}

//явное компилирование экземпляра шаблона
template std::ostream& operator<<(std::ostream& out, const std::vector<Rectangle>& a);