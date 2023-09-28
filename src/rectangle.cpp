#include "rectangle.h"


const unsigned MAIN_SIDE_WIDTH = 56;
const unsigned END_OF_VARIATION = (1 << 12); // 1 0000 0000 0000 � ��������

std::ostream& operator<<(std::ostream& out, const Rectangle& c) {
	out << '[' << c[0] << "x" << c[1] << ']';
	return out;
}

std::ostream& operator<<(std::ostream& out, const RectanglePlacement& c) {
	out << '[' << (c.x2 - c.x1) << "x" << (c.y2 - c.y1) << ']';
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


unsigned RectanglePlacement::Weight() const
{
	return x1 * 11 + y1 * 7 + x2 * 5 + y2 * 3;
}

bool RectanglePlacement::operator<(const RectanglePlacement& other) const
{
	return Weight() < other.Weight();
}


unsigned GetNumberSide(unsigned variation, unsigned number) {
	number = 12 - 1 - number;
	auto side_number = (variation >> number) % 2;
	return side_number;
}

unsigned TearTail(unsigned variation, unsigned number) {
	//���������� ��������� �������� �� ���������
	//������� ������� (number) �� �������, � ������ �������� (12 - number)
	if (11 < number)
	{
		return variation;
	}
	unsigned variation_add = 1;
	auto number_bit = 12 - number;
	for (size_t i = 0; i < number_bit; i++)
	{
		variation |= variation_add; //��������� ���
		variation_add <<= 1; //�������� �������� �� ���� ������
	}
	return variation;
}

bool Inside(const Placement& pl, const RectanglePlacement& rect_pl) {
	return (rect_pl.x1 <= pl[0] && pl[0] < rect_pl.x2 && rect_pl.y1 <= pl[1] && pl[1] < rect_pl.y2); //����� ������ ��������������, �� �� �������
}

bool PlacementIsBusy(const Placement& pl, const Calibron12Box& cb) {
	for (const auto& i : cb) //�� ������ �������
	{
		for (const auto& j : i) // ������ �������������
		{
			if (Inside(pl, j))
			{
				return  true; //����� ������ ��������������, �� �� �������
			}
		}
	}
	return false;
}


//���������� ������, ���� 
//��������� �� ��������� �����
//���� ����� ������, �� �������� �� ��������� ��� �� ����, ��������� ����
bool FirstFree(Placement& pl, bool vertically, const Calibron12Box& rect_plasements) {
	//++pl[vertically];//�������� ������ �� �������, ������� �� ��������� �������
	bool search_has_changed = true;
	//���������� �����, ����� � ���������. ��� ������, 
	// ���� ������� ���������, ���� ����� �������
	while (search_has_changed && pl[vertically] < MAIN_SIDE_WIDTH)
	{
		search_has_changed = false; //���� �������� ���������, �� ������ �� �����
		for (int i = 0; i < 4 && !search_has_changed && pl[vertically] < MAIN_SIDE_WIDTH; i++) //�� ������ �������
		{
			for (const auto& j : rect_plasements[i]) // ������ �������������
			{
				if (Inside(pl, j)) //����� ������ ��������������, �� �� �������
				{
					//������������� �� �������������
					pl[vertically] = (vertically ? j.y2 : j.x2); //�� ��������� ����� ����� ��������������
					//������ ����� �� i
					i = -1;
					search_has_changed = true;
					//�� ���������, �������������, ������ ����� ����� ��������� ��
					// ������ ����� while 
					break;
				}
			}
		}
	}
	return pl[vertically] < MAIN_SIDE_WIDTH;
}

//���������� ������, ���� 
//��������� �� ��������� ����������� �����
// �� ��������� - ���� ������, �� �� ������ �� �������, 
//���� ����� ��������, �� �������� �� ���������, ��������� ����
bool LastFree(Placement& pl, bool vertically, const Calibron12Box& rect_plasements) {
	//	++pl[vertically];//�������� ������ �� ���������, ������� �� ��������� �������
	while (pl[vertically] < MAIN_SIDE_WIDTH)
	{
		for (size_t i = 0; i < 4; i++) //�� ������ �������, ���� �� ������� ����� ��������
		{
			for (const auto& j : rect_plasements[i]) // ������ �������������
			{
				if (Inside(pl, j)) //����� ������ ��������������, �� �� �������
				{
					//���������� �� �������������
					return true;
				}
			}
		}
		//��������� ����� ���������
		++pl[vertically];
	}
	//��������� �� ����� ��� ������ ���� ������, �������� ������
	//���������� ����� �� ������� ����.
	assert(false);
	return false; //�����
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
// ��������� ��� RectanglePlacement 
// x1 < x2, y1 < y2;
bool IsIntersectAB(const RectanglePlacement& a, const RectanglePlacement& b)
{
	// a ������ ��� �����
	if (a.x1 >= b.x2 || a.x2 <= b.x1)
	{
		return false;
	}
	// ������ ���� �����, ���� ������ ���� a ����� b.x1 ��� b.x2
	// � ���� ���� ���� �� ������������ a ��������� ����� ������������� b, ������ ���� �����������
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
	// � ����� ������ �� ����� ����, ����� ���� ��������� ��������� ������
	// �.�. ����������� ������ ���������� � ���������� ��� ���������������
	// ����� ������ ����������� ������
	// ���������� ����� ����������� ���� �� ���������, ���� �� �����������
	// ���� ���� �����������, �� ���� a ���������� b �� �����������, ���� b ���������� a
	// ��������� ������ ������������ �����������, a -> b ����� b -> a
	//if (a.x2 <= MAIN_SIDE_WIDTH && a.y2 <= MAIN_SIDE_WIDTH && b.x2 <= MAIN_SIDE_WIDTH && b.y2 <= MAIN_SIDE_WIDTH)
	//{
		return (IsIntersectAB(a, b) || IsIntersectAB(b, a));
	//}
	//return true;
}

	bool NextFreePosition(Placement& pl, const CalibronBox& rect_plasements) {
		while (pl[1] < MAIN_SIDE_WIDTH)
		{
			bool search_complited = true;
			for (const auto& j : rect_plasements) // ������ �������������
			{
				if (Inside(pl, j)) //����� ������ ��������������, �� �� �������
				{
					//������������� �� �������������
					if (j.x2 < MAIN_SIDE_WIDTH)
					{
						pl[0] = j.x2; //�� ��������� ����� ����� ��������������
					}
					else { //��� ��� � ������ ��������� ������
						pl[0] = 0;
						++pl[1];
					}
					search_complited = false;
					break;
				}
			}
			if (search_complited)
			{
				return true;
			}
		}
		//��������� ����� ������� ����.
		assert(false);
		return false;
	}
	bool Fit(const RectanglePlacement& rect, const CalibronBox& rect_plasements) {
		//����� �� �������
		if ((rect.x2 > MAIN_SIDE_WIDTH) || (rect.y2 > MAIN_SIDE_WIDTH))
		{
			return false;
		}
		
		for (const auto& it : rect_plasements) {
			if (IsIntersect(it, rect))
			{
				return false;
			}
		}
		return true;
	}


//����� �������������� ���������� �������
	template std::ostream& operator<<(std::ostream& out, const std::vector<Rectangle>& a);
	template std::ostream& operator<<(std::ostream& out, const CalibronBox& a);
	
