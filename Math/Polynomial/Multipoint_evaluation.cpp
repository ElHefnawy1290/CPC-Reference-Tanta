/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MULTIPOINT EVALUATION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Evaluate polynomial at multiple points", "P(x_i)".
 * - Classic Scenarios: You have a massive polynomial P(x) of degree N, and you need
 *   to find its exact value at M different points (Topic 340).
 * - The Magic: Horner's method takes O(N * M) which TLEs. This uses a Subproduct Tree
 *   (a segment tree of polynomials) and polynomial division with remainder to process
 *   all queries simultaneously in O(N log^2 N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<ll> results = multipoint_eval(P, X);
 *       // results[i] contains P(X[i]) mod 998244353.
 *
 * - Complexity:
 *       Time: O(N log^2 N)
 *       Space: O(N log N)
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Polynomial Modulo: This struct includes `poly_mod(A, B)` which finds the remainder
 *   of A(x) / B(x). This is an incredibly powerful tool on its own.
 */

vector<ll> poly_rev(vector<ll> a)
{
    reverse(a.begin(), a.end());
    return a;
}

vector<ll> poly_mod(vector<ll> a, vector<ll> b)
{
    while (a.size() > 1 && a.back() == 0)
        a.pop_back();
    while (b.size() > 1 && b.back() == 0)
        b.pop_back();
    if (a.size() < b.size())
        return a;

    int n = a.size(), m = b.size();
    vector<ll> rev_a = poly_rev(a);
    vector<ll> rev_b = poly_rev(b);

    rev_a.resize(n - m + 1);
    vector<ll> inv_rev_b = poly_inv(rev_b, n - m + 1);
    vector<ll> q = multiply(rev_a, inv_rev_b);
    q.resize(n - m + 1);
    q = poly_rev(q);

    vector<ll> qb = multiply(q, b);
    vector<ll> r(m - 1, 0);
    for (int i = 0; i < m - 1; i++)
    {
        r[i] = (a[i] - (i < qb.size() ? qb[i] : 0)) % MOD;
        if (r[i] < 0)
            r[i] += MOD;
    }
    while (r.size() > 1 && r.back() == 0)
        r.pop_back();
    return r;
}

vector<vector<ll>> tree;

void build_subproduct_tree(int node, int l, int r, const vector<ll> &x)
{
    if (l == r)
    {
        tree[node] = {(MOD - x[l]) % MOD, 1};
        return;
    }
    int mid = l + (r - l) / 2;
    build_subproduct_tree(2 * node, l, mid, x);
    build_subproduct_tree(2 * node + 1, mid + 1, r, x);
    tree[node] = multiply(tree[2 * node], tree[2 * node + 1]);
}

void eval_subproduct_tree(int node, int l, int r, vector<ll> p, vector<ll> &ans)
{
    p = poly_mod(p, tree[node]);
    if (l == r)
    {
        ans[l] = p.empty() ? 0 : p[0];
        return;
    }
    int mid = l + (r - l) / 2;
    eval_subproduct_tree(2 * node, l, mid, p, ans);
    eval_subproduct_tree(2 * node + 1, mid + 1, r, p, ans);
}

vector<ll> multipoint_eval(const vector<ll> &p, const vector<ll> &x)
{
    if (x.empty())
        return {};
    int m = x.size();
    tree.resize(4 * m);
    build_subproduct_tree(1, 0, m - 1, x);
    vector<ll> ans(m);
    eval_subproduct_tree(1, 0, m - 1, p, ans);
    return ans;
}