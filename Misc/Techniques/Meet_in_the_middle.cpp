#include <bits/stdc++.h>
using namespace std;

// This template solves the classic "Find if any subset sums to Target"
// Complexity: O(2^(N/2) * N)

void generate_sums(const vector<long long> &arr, vector<long long> &sums)
{
    int n = arr.size();
    // Use bitmasking to generate all 2^n subsets
    for (int i = 0; i < (1 << n); i++)
    {
        long long current_sum = 0;
        for (int j = 0; j < n; j++)
        {
            if ((i >> j) & 1)
            {
                current_sum += arr[j];
            }
        }
        sums.push_back(current_sum);
    }
}

void solve()
{
    int n;
    long long target;
    cin >> n >> target;

    vector<long long> arr(n);
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // 1. Split into two halves
    vector<long long> left_part, right_part;
    for (int i = 0; i < n; i++)
    {
        if (i < n / 2)
            left_part.push_back(arr[i]);
        else
            right_part.push_back(arr[i]);
    }

    // 2. Generate all subset sums for both halves
    vector<long long> left_sums, right_sums;
    generate_sums(left_part, left_sums);
    generate_sums(right_part, right_sums);

    // 3. Sort one half for binary searching
    sort(left_sums.begin(), left_sums.end());

    // 4. Search for the complement in the other half
    bool found = false;
    for (long long s : right_sums)
    {
        long long needed = target - s;
        // Check if 'needed' exists in left_sums
        if (binary_search(left_sums.begin(), left_sums.end(), needed))
        {
            found = true;
            break;
        }
    }

    if (found)
        cout << "YES\n";
    else
        cout << "NO\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
