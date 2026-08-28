/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DYNAMIC STRING HASHING (SEGMENT TREE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Dynamic Palindrome Queries", "Substring Equality with Point Updates", "Double Hashing".
 * - Classic Scenarios: You have a string and you need to answer queries like "Is the substring [L, R] 
 *   a palindrome?" OR "Does substring [L1, R1] match [L2, R2]?". Crucially, the string is NOT static; 
 *   you will receive point updates (changing the character at index X).
 * - The Magic: "Polynomial Rolling Hash inside a Segment Tree". A rolling hash is just a sum of terms 
 *   (char * base^i). Segment trees are perfect for point updates and range sums! By maintaining the hash 
 *   in a Segment Tree, updates take O(log N). To query a range [L, R], we sum the hashes in that range 
 *   and "normalize" it by multiplying by the modular inverse of base^(L-1).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization (CRITICAL): You MUST call `init()` once at the start of `main()` to precompute 
 *   the powers and modular inverses.
 * - 1-Based Indexing ONLY: Your string indices and queries MUST be 1-based (from 1 to N).
 *       HashingSegmentTree forward_tree(N), backward_tree(N);
 * - Building: Add characters one by one. For the backward tree, insert at `N - idx + 1`.
 *       forward_tree.update(i, s[i]);
 *       backward_tree.update(n - i + 1, s[i]);
 * - Execution: 
 *       To update a char: forward_tree.update(idx, new_c); backward_tree.update(n - idx + 1, new_c);
 *       To check palindrome: isPalindrome(forward_tree, backward_tree, L, R, n);
 *
 * - Complexity:
 *       Time: Initialization O(N), Updates O(log N), Queries O(log N).
 *       Space: O(N) for the precomputed arrays and Segment Tree nodes.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - ⚠️ 0-Based Indexing Crash Warning: The `query` function uses `inv1[l - 1]`. If you pass `l = 0`, 
 *   it will access index `-1` and cause Undefined Behavior. ALWAYS use 1-based indexing for queries.
 * - Hack Prevention: This template uses Double Hashing with primes 1e9+7 and 2e9+11. It is virtually 
 *   impossible to break (anti-hash tests will fail). If you face a very strict Time Limit (TLE), you 
 *   can convert it to Single Hashing (remove mod2, base2, pw2, inv2) and use a randomized base instead.
 * - `#define int long long`: This template heavily relies on this macro. If you ever remove it, you 
 *   MUST change all hash values, modulos, and products to `long long` to prevent integer overflow 
 *   during `(A * B) % MOD`.
 */

#define int long long
const int N = 1e5 + 5, mod1 = 1e9 + 7, mod2 = 2e9 + 11;
ll base1 = 31, base2 = 37, pw1[N + 1], pw2[N + 1], inv1[N + 1], inv2[N + 1];

ll powmod(ll a, ll b, ll m)
{
    ll ans = 1;
    while (b > 0)
    {
        if (b & 1)
        {
            ans = (ans * a) % m;
        }
        a = (a * a) % m;
        b >>= 1;
    }
    return ans;
}

void init()
{
    pw1[0] = pw2[0] = inv1[0] = inv2[0] = 1;
    int temp1 = powmod(base1, mod1 - 2, mod1);
    int temp2 = powmod(base2, mod2 - 2, mod2);
    for (int i = 1; i < N; i++)
    {
        pw1[i] = (base1 * pw1[i - 1]) % mod1;
        pw2[i] = (base2 * pw2[i - 1]) % mod2;
        inv1[i] = (inv1[i - 1] * temp1) % mod1;
        inv2[i] = (inv2[i - 1] * temp2) % mod2;
    }
}

struct HashingSegmentTree
{
private:
    vector<pair<int, int>> seg;
    int sz;

    pair<int, int> merge(pair<int, int> l, pair<int, int> r)
    {
        pair<int, int> ret = l;
        ret.first = (ret.first + r.first) % mod1;
        ret.second = (ret.second + r.second) % mod2;
        return ret;
    }

    void update(int l, int r, int node, int idx, int ch)
    {
        if (l == r)
        {
            seg[node] = {(ch * pw1[idx]) % mod1, (ch * pw2[idx]) % mod2};
            return;
        }
        int mid = l + r >> 1;
        if (idx <= mid)
            update(l, mid, 2 * node + 1, idx, ch);
        else
            update(mid + 1, r, 2 * node + 2, idx, ch);
        seg[node] = merge(seg[2 * node + 1], seg[2 * node + 2]);
    }

    pair<int, int> query(int l, int r, int node, int lx, int rx)
    {
        if (l >= lx && r <= rx)
        {
            return seg[node];
        }
        if (l > rx || r < lx)
            return {0, 0};
        int mid = l + r >> 1;
        pair<int, int> lft = query(l, mid, 2 * node + 1, lx, rx);
        pair<int, int> rgt = query(mid + 1, r, 2 * node + 2, lx, rx);
        return merge(lft, rgt);
    }

public:
    HashingSegmentTree(int n)
    {
        sz = 1;
        while (sz <= n)
            sz *= 2;
        seg = vector<pair<int, int>>(sz * 2);
    }

    void update(int idx, char ch)
    {
        update(0, sz - 1, 0, idx, ch - 'a' + 1);
    }

    pair<int, int> query(int l, int r)
    {
        pair<int, int> ret = query(0, sz - 1, 0, l, r);
        ret.first = (ret.first * inv1[l - 1]) % mod1;
        ret.second = (ret.second * inv2[l - 1]) % mod2;
        return ret;
    }
};

bool isPalindrome(HashingSegmentTree &a, HashingSegmentTree &b, int &l, int &r, int &n)
{
    return (a.query(l, r) == b.query(n - r + 1, n - l + 1));
}
