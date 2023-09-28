#include <algorithm>
#include "Solution.h"
#include "render_rectangle.h"

Solution::Solution(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned variation, unsigned number)
	: number_(number), rs_(rs.size())
{
	//assert(12 > number);
	for (size_t i = 0; i < rs.size(); i++)
	{
		if (GetNumberSide(variation, i)) {
			rs_[i][0] = rs[order[i]][1];
			rs_[i][1] = rs[order[i]][0];
		}
		else {
			rs_[i] = rs[order[i]];
		}
	}
}

bool Solution::operator==(const Solution& other) const {
	//хвост не анализируем, только те, что вдоль краёв
	if (number_ != other.number_)
	{
		return false;
	}
	for (size_t i = 0; i < number_; i++)
	{
		if (rs_[i] != other.rs_[i]) {
			return false;
		}
	}
	return true;
}



std::ostream& operator<<(std::ostream& out, const Solution& s)
{
	unsigned sum = 0;
	unsigned sum_start = s.rs_[0][1];
	out << '{';
	for (size_t i = 0; i < s.rs_.size(); i++)
	{
		sum += s.rs_[i][0];
		out << s.rs_[i] << '(' << sum << ')' << ',';
		if (sum == 56)
		{
			sum = s.rs_[i][1];
		}
		if (i == s.number_ - 1) //последняя по последнему краю
		{
			sum += sum_start;
			out << " < " << sum << " > ";
			sum = 0;
		}
	}
	out << '}';	return out;
}

bool Solutions::Add(Solution&& solution) {
	if (std::find(solutions_.cbegin(), solutions_.cend(), solution) == solutions_.cend())
	{
		solutions_.push_back(std::move(solution));
		return true;
	}
	return false;
};
