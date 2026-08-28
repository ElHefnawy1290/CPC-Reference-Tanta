#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: CDQ DIVIDE AND CONQUER
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "3D Partial Order", "Dynamic 2D Range Queries", "Offline range counting".
 * - Classic Scenarios: You need to count how many points i have (x_i <= X, y_i <= Y, z_i <= Z).
 * - The Magic: CDQ D&C reduces dimensionality.
 *   - Dimension 1 (X): Handled by sorting the array.
 *   - Dimension 2 (Y): Handled by the Divide and Conquer recursion.
 *   - Dimension 3 (Z): Handled by a 1D Fenwick Tree (BIT).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Define a struct Element {x, y, z, id}.
 * - Sort the elements initially by x.
 * - Call cdq(0, n-1) to process.
 *
 * - Complexity:
 *       Time: O(N log^2 N) where N is the number of elements.
 *       Space: O(N) memory.
 */

struct Element
{
    int x, y, z, id;
};

// 1D Fenwick Tree for the 3rd dimension
struct BIT
{
    int n;
    vector<int> tree;
    BIT(int n) : n(n), tree(n + 1, 0) {}

    void update(int i, int delta)
    {
        for (; i <= n; i += i & -i)
            tree[i] += delta;
    }

    int query(int i)
    {
        int sum = 0;
        for (; i > 0; i -= i & -i)
            sum += tree[i];
        return sum;
    }
};

int n;
vector<Element> arr;
vector<int> result;
BIT bit(1000005); // Adjust size to coordinate range

void cdq(int l, int r)
{
    if (l >= r)
        return;
    int mid = l + (r - l) / 2;

    cdq(l, mid);
    cdq(mid + 1, r);

    // Sort both halves by Y (Dimension 2)
    vector<Element> temp;
    int i = l, j = mid + 1;

    while (i <= mid && j <= r)
    {
        if (arr[i].y <= arr[j].y)
        {
            bit.update(arr[i].z, 1);
            temp.push_back(arr[i++]);
        }
        else
        {
            result[arr[j].id] += bit.query(arr[j].z);
            temp.push_back(arr[j++]);
        }
    }

    // Process remaining
    while (j <= r)
    {
        result[arr[j].id] += bit.query(arr[j].z);
        temp.push_back(arr[j++]);
    }

    // Clear BIT before moving on
    for (int k = l; k < i; k++)
        bit.update(arr[k].z, -1);

    while (i <= mid)
        temp.push_back(arr[i++]);
    for (int k = 0; k < temp.size(); k++)
        arr[l + k] = temp[k];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Example: Read N elements with (x, y, z) coordinates
    // Ensure all coordinates are mapped to 1-indexed values for BIT

    // cdq(0, n - 1);

    return 0;
}