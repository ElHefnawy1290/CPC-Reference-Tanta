/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: WAVELET TREE
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "K-th smallest in range", "Range frequency count", "Count elements < K".
 * - Classic Scenarios: You have an array and you need to answer queries like:
 *   "Find the k-th smallest element in subarray [L, R]" or "Count occurrences
 *   of values in [A, B] within index range [L, R]".
 * - The Magic: It is essentially a Merge Sort Tree, but instead of storing
 *   sorted vectors (which consume O(N log N) memory), it stores a bit-vector
 *   representation of the partitioning. This allows queries in O(log(MaxVal)) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       // Pass the range of values [min_val, max_val] and the input vector
 *       WaveletTree wt(1, 1000000, array);
 *
 * - Queries:
 *       // How many numbers < K in subarray [L, R]
 *       int count = wt.count_less(L, R, K);
 *       // Find the k-th smallest number in subarray [L, R] (1-indexed k)
 *       int val = wt.kth_smallest(L, R, k);
 *
 * - Complexity:
 *       Time: O(N log(MaxVal)) to build. O(log(MaxVal)) per query.
 *       Space: O(N log(MaxVal)) memory.
 */

#include <bits/stdc++.h>
using namespace std;

struct WaveletTree
{
    int low, high;
    WaveletTree *left, *right;
    vector<int> pref; // Prefix sums of elements going to the left child

    WaveletTree(int l, int r, const vector<int> &a) : low(l), high(r), left(nullptr), right(nullptr)
    {
        if (low == high || a.empty())
            return;

        int mid = low + (high - low) / 2;

        // Define function to check if value goes to left child
        auto is_left = [&](int x)
        { return x <= mid; };

        pref.reserve(a.size() + 1);
        pref.push_back(0);

        vector<int> l_vec, r_vec;
        for (int x : a)
        {
            if (is_left(x))
            {
                l_vec.push_back(x);
                pref.push_back(pref.back() + 1);
            }
            else
            {
                r_vec.push_back(x);
                pref.push_back(pref.back());
            }
        }

        left = new WaveletTree(low, mid, l_vec);
        right = new WaveletTree(mid + 1, high, r_vec);
    }

    // Count numbers <= k in range [ql, qr] (0-indexed)
    int count_less_equal(int ql, int qr, int k)
    {
        if (ql > qr || low > k)
            return 0;
        if (high <= k)
            return qr - ql + 1;

        int l_count_before = pref[ql];
        int r_count_before = ql - l_count_before;
        int l_count_all = pref[qr + 1];
        int r_count_all = (qr + 1) - l_count_all;

        return left->count_less_equal(l_count_before, l_count_all - 1, k) +
               right->count_less_equal(r_count_before, r_count_all - 1, k);
    }

    // Find k-th smallest number in range [ql, qr]
    int kth_smallest(int ql, int qr, int k)
    {
        if (low == high)
            return low;

        int l_count_before = pref[ql];
        int r_count_before = ql - l_count_before;
        int l_count_all = pref[qr + 1];
        int r_count_all = (qr + 1) - l_count_all;

        int left_size = l_count_all - l_count_before;

        if (k <= left_size)
            return left->kth_smallest(l_count_before, l_count_all - 1, k);
        else
            return right->kth_smallest(r_count_before, r_count_all - 1, k - left_size);
    }
};