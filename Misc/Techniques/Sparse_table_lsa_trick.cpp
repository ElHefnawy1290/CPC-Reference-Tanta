/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SPARSE TABLE (LSA TRICK)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Range Maximum Query", "Immutable range query".
 * - Classic Scenarios: You have an array that never changes, and you need to
 *   query the maximum/minimum in any range [L, R] in O(1).
 * - The Magic: Precomputes logs for all ranges of length 2^k. Since `max` is
 *   idempotent (max(a, a) = a), we can answer queries by taking the max of two
 *   overlapping 2^k blocks that cover the entire [L, R].
 */

const int K = 20; // Enough for 10^6 elements (2^20 > 10^6)
int st[K][100005];
int lg[100005];

void build_sparse_table(const vector<int> &a)
{
    int n = a.size();
    lg[1] = 0;
    for (int i = 2; i <= n; i++)
        lg[i] = lg[i / 2] + 1;
    for (int i = 0; i < n; i++)
        st[0][i] = a[i];

    for (int j = 1; j < K; j++)
    {
        for (int i = 0; i + (1 << j) <= n; i++)
        {
            st[j][i] = max(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
        }
    }
}

int query(int L, int R)
{
    int j = lg[R - L + 1];
    return max(st[j][L], st[j][R - (1 << j) + 1]);
}