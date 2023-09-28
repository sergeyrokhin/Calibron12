#pragma once

#include "rectangle.h"

class Solution {
	//количество прямоугольников вдоль краёв
	unsigned number_ = 0;
	RectangleSet rs_;
public:

	Solution() = delete;
	Solution(const RectangleSet& rs, const std::vector<unsigned>& order, unsigned variation, unsigned number);
	bool operator== (const Solution& other) const;
	friend std::ostream& operator<<(std::ostream& out, const Solution& s);
};

class Solutions
{
public:
	Solutions() = default;
	bool Add(Solution&& solution);
	std::vector<Solution>::iterator begin() { return solutions_.begin(); };
	std::vector<Solution>::iterator end() { return solutions_.end(); };
	Solution& back() { return solutions_.back(); };
private:
	std::vector<Solution> solutions_;
};


void FindSolutions();