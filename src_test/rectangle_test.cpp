#include <numeric>
#include <cassert>
#include "rectangle_test.h"

void TestRectangleSet(const RectangleSet& rect_) {
	assert(MAIN_SIDE_WIDTH * MAIN_SIDE_WIDTH ==
		std::accumulate(rect_.begin(), rect_.end(), 0, [](int sum, const Rectangle& r) { return sum + r[0] * r[1]; }));
}

bool IsIntersectAB(const RectanglePlacement& a, const RectanglePlacement& b);

void TestIsIntersect() {
	{
		//			 3   5    9       13
		//		+--------+
		//		|A       |
		//		|        |
		//	4   |        |     +-------+
		//		|        |     |   B   |
		//	6	|        |     +-------+
		//		|        |
		//	8	+--------+ 

		RectanglePlacement A(0, 5, 0, 8), B(9, 13, 4, 6);
		assert( ! IsIntersectAB(A, B)); 
		assert( ! IsIntersectAB(B, A));
	}

	{
		//	         3   5    9
		//	    +--------+
		//	    |A       |
		//		|        |
		//	4   |    +---|----+
		//      |    |   |  B |
		//  6   |    |   |    |
		//      |    |   |    |
		//  8   +----|---+    | 
		//           |        |
		//	10		 +--------+	

		RectanglePlacement A(0, 5, 0, 8), B(3, 9, 4, 10);
		// B пересекает по горизонтали A
		// И наоборот
		assert(IsIntersectAB(A, B));
		assert(IsIntersectAB(B, A));
	}

	{
		//			 3   5    9
		//		+--------+
		//		|A       |
		//		|        |
		//	4   |    +---|----+
		//		|    |   |  B |
		//	6	|    +---|----+
		//		|        |
		//	8	+--------+ 

		RectanglePlacement A(0, 5, 0, 8), B(3, 9, 4, 6);
		// B пересекает по горизонтали A
		// но не наоборот
		assert( ! IsIntersectAB(A, B));
		assert(IsIntersectAB(B, A));
	}

	{
		//			 3   5    9
		//		+--------+
		//		|A       |
		//		|        |
		//	4   +--------|----+
		//		|        |  B |
		//	6	+--------|----+
		//		|        |
		//	8	+--------+ 

		RectanglePlacement A(0, 5, 0, 8), B(0, 9, 4, 6);
		// B пересекает по горизонтали A
		// но не наоборот
		assert( ! IsIntersectAB(A, B));
		assert(IsIntersectAB(B, A));
	}

	{
		//	  0 1	 3   5    9
		//		+--------+
		//		|A       |
		//		|        |
		//	4 +----------|----+
		//	  |	|        |  B |
		//	6 +----------|----+
		//		|        |
		//	8	+--------+ 

		RectanglePlacement A(1, 5, 0, 8), B(0, 9, 4, 6);
		// B пересекает по горизонтали A
		// но не наоборот
		assert( ! IsIntersectAB(A, B));
		assert(IsIntersectAB(B, A));
	}
}
