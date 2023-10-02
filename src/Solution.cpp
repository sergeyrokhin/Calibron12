#include <algorithm>
#include "Solution.h"
#include "render_rectangle.h"

Solution::Solution(const CalibronBox& rectangles)
	: rectangles_(rectangles)
{
	std::sort(rectangles_.begin(), rectangles_.end());
}

bool Solution::operator==(const Solution& other) const {
	//хвост не анализируем, только те, что вдоль краёв
	auto size = rectangles_.size();
	if (size != other.rectangles_.size())
	{
		return false;
	}
	for (size_t i = 0; i < size; i++)
	{
		if (
			rectangles_[i].x1 != other.rectangles_[i].x1 ||
			rectangles_[i].x2 != other.rectangles_[i].x2 ||
			rectangles_[i].y1 != other.rectangles_[i].y1 ||
			rectangles_[i].y2 != other.rectangles_[i].y2
			) {
			return false;
		}
	}
	return true;
}



std::ostream& operator<<(std::ostream& out, const Solution& s)
{
	out << s.rectangles_;
	return out;
}

bool Solutions::Add(Solution&& solution) {
	if (std::find(solutions_.cbegin(), solutions_.cend(), solution) == solutions_.cend())
	{
		solutions_.push_back(std::move(solution));
		return true;
	}
	return false;
};
