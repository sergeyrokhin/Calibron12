#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <cassert>
#include <optional>

extern const unsigned MAIN_SIDE_WIDTH;
extern const unsigned END_OF_VARIATION;

using Rectangle = std::array<unsigned int, 2>;
using Placement = std::array<unsigned int, 2>;

using RectangleSet = std::vector<Rectangle>;

// ��������� ��� RectanglePlacement 
// x1 < x2, y1 < y2;
class RectanglePlacement {
public:
	unsigned x1 =0, x2 = 0, y1 = 0, y2 = 0;

	RectanglePlacement() = default;
	//�� �������� �������������� � �����������
	RectanglePlacement(Rectangle rect, Placement c_xy) : x1(c_xy[0]), x2(x1 + rect[0]), y1(c_xy[1]), y2(y1 + rect[1]) {	}
	//�� �������� ��������������, � ����� ������� ������, �������� �� ������ �������
	RectanglePlacement(Rectangle rect, unsigned side_number, unsigned x0);

	RectanglePlacement(unsigned x1, unsigned x2, unsigned y1, unsigned y2)
		: x1(x1), x2(x2), y1(y1), y2(y2)
	{
	}
	unsigned Weight() const;
	bool operator<(const RectanglePlacement& other) const;
};

using Calibron12Box = std::array<std::vector<RectanglePlacement>, 4>;
using CalibronBox = std::vector<RectanglePlacement>;

std::ostream& operator<<(std::ostream& out, const Rectangle& c);

//���� ������ �������������� ���������, � �������� �������� � ������
// �� ����� ���� ������ �� ��������, �� ��� ������ �������������� ���������
unsigned GetNumberSide(unsigned variation, unsigned number);

//���� ������ �������������� ���������, � �������� �������� � ������
// �� ����� ���� ������ �� ��������, �� ��� ������ �������������� ���������
//���������� ��������� �������� �� ���������
//������� ������� (number) �� �������, � ������ "����������" (12 - number)
unsigned TearTail(unsigned variation, unsigned number);

std::pair<unsigned, unsigned> GetRectangle(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned variation, unsigned number);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& a);

bool IsIntersect(const RectanglePlacement &a, const RectanglePlacement &b);
bool NextFreePosition(Placement& pl, const CalibronBox& rect_plasements);
bool Fit(const RectanglePlacement& rect, const CalibronBox& rect_plasements);
