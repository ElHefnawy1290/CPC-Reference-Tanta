/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MANHATTAN <-> CHEBYSHEV
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Rotate coordinate system", "Simplify absolute value distance".
 * - Classic Scenarios: A problem asks you to find the number of points within
 *   Manhattan distance D from (X, Y). This is an "octagonal" area, which is
 *   extremely hard to query.
 * - The Magic:
 *   Transformation: (x, y) -> (x+y, x-y)
 *   Manhattan distance in the original plane is equivalent to Chebyshev distance
 *   in the transformed plane.
 *   Chebyshev distance: max(|x1 - x2|, |y1 - y2|) creates a *square* area, which
 *   is trivially solvable using standard 2D Segment Trees or Fenwick Trees.
 *
 * 2. 📦 HOW TO USE IT
 * - Transform:
 *       new_x = x + y;
 *       new_y = x - y;
 */