/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SMAWK ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Totally monotone matrix", "Min-plus convolution", "O(N) optimization".
 * - Classic Scenarios: You need to find the minimum value in every row of an N x M
 *   matrix where the matrix satisfies the Monge property (or is totally monotone).
 * - The Magic: Bypasses standard O(N M) brute force and even O(N log M) Divide &
 *   Conquer. It recursively filters out columns that can never contain a row minimum,
 *   solving the entire problem in strictly O(N + M) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<int> min_indices = smawk(R, C, cost_function);
 * - Result:
 *       Returns an array `ans` of size R, where `ans[i]` is the column index
 *       containing the minimum value for row i.
 */

using ll = long long;

template <class F>
vector<int> smawk(int r, int c, F cost)
{
    auto solve = [&](auto &self, const vector<int> &rows, const vector<int> &cols) -> vector<int>
    {
        if (rows.empty())
            return {};

        // Reduce: filter columns
        vector<int> st;
        for (int c : cols)
        {
            while (!st.empty())
            {
                if (cost(rows[st.size() - 1], st.back()) > cost(rows[st.size() - 1], c))
                {
                    st.pop_back();
                }
                else if (st.size() < rows.size())
                {
                    break;
                }
                else
                {
                    goto next_col;
                }
            }
            st.push_back(c);
        next_col:;
        }

        vector<int> odd_rows;
        for (int i = 1; i < rows.size(); i += 2)
            odd_rows.push_back(rows[i]);

        // Solve recursively for odd-indexed rows
        vector<int> odd_ans = self(self, odd_rows, st);
        vector<int> ans(rows.size());
        for (int i = 0; i < odd_ans.size(); i++)
            ans[2 * i + 1] = odd_ans[i];

        // Interpolate for even-indexed rows
        int c_idx = 0;
        for (int i = 0; i < rows.size(); i += 2)
        {
            int end_c = (i + 1 == rows.size()) ? st.back() : ans[i + 1];
            int best_c = st[c_idx];
            ll best_val = cost(rows[i], best_c);

            while (st[c_idx] != end_c)
            {
                c_idx++;
                ll val = cost(rows[i], st[c_idx]);
                if (val < best_val)
                {
                    best_val = val;
                    best_c = st[c_idx];
                }
            }
            ans[i] = best_c;
        }
        return ans;
    };

    vector<int> rows(r), cols(c);
    iota(rows.begin(), rows.end(), 0);
    iota(cols.begin(), cols.end(), 0);
    return solve(solve, rows, cols);
}