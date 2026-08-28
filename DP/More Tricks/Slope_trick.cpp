/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SLOPE TRICK
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Minimum operations to make array non-decreasing".
 * - Classic Scenarios: You can increment or decrement elements of an array.
 *   Find the minimum total cost to make the array sorted (non-decreasing).
 * - The Magic: A 2D DP takes O(N * MAX_VAL) which TLEs. By observing that the
 *   DP cost function is convex and piecewise linear, we can just maintain the
 *   points where the slope changes using a max-heap.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       long long min_cost = make_non_decreasing(A);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Strictly Increasing: If the problem requires strictly increasing (A[i] < A[i+1]),
 *   subtract `i` from every element initially: `A[i] -= i`, then run this exact
 *   function. It maps the strictly increasing problem directly to a non-decreasing one.
 *
 * - Complexity:
 *       Time: O(N log N)
 *       Space: O(N)
 */

using ll = long long;

ll make_non_decreasing(const vector<ll> &a)
{
    priority_queue<ll> pq;
    ll total_cost = 0;

    for (int i = 0; i < a.size(); i++)
    {
        pq.push(a[i]);

        // If the max element so far is strictly greater than the current element,
        // it means we have a slope violation. We "flatten" it to match A[i].
        if (pq.top() > a[i])
        {
            total_cost += pq.top() - a[i];
            pq.pop();
            pq.push(a[i]);
        }
    }

    return total_cost;
}