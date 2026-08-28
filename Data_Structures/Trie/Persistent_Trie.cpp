/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: PERSISTENT BINARY TRIE (XOR MAXIMIZER)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Maximum XOR in a subarray", "Range XOR", "Subsegment XOR with X".
 * - Classic Scenarios: You are given an array of N integers. You need to answer Q queries.
 *   Each query gives you a range [L, R] and a value X. You need to find an element Y
 *   in the subarray from L to R such that (Y ^ X) is maximized.
 * - The Magic: A standard Trie can find the max XOR in a whole array. By making it Persistent,
 *   we save a new "version" of the Trie after inserting every single element. To query the
 *   range [L, R], we take the Trie at version R and mathematically subtract the paths from
 *   version L-1. It does this in blindingly fast O(30) time!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass a 0-indexed vector to build the historical versions.
 *       vector<long long> a = {3, 10, 5, 25, 2};
 *       PersistentTrie ptrie(a);
 *
 * - Queries (All 0-indexed, Inclusive [L, R]):
 *       // Find the maximum XOR of 7 with any element in the subarray from index 1 to 3
 *       long long ans = ptrie.queryMaxXor(1, 3, 7);
 *
 * - Complexity:
 *       Time: O(N * BITS) to build. O(BITS) per query.
 *       Space: O(N * BITS) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Dealing with huge numbers (up to 10^18)?
 *   Change the `BITS` constant from 30 to 60. (30 is enough for numbers up to 10^9).
 * - Minimum XOR instead of Maximum XOR?
 *   I have included a `queryMinXor` function right below the Max one! It simply tries to
 *   take the path of the SAME bit instead of the FLIPPED bit.
 * - Need to dynamically add elements to the back of the array?
 *   Just call `ptrie.insertBack(X);`. It will add the element and create a new version
 *   that you can instantly query.
 */

#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie
{
private:
    // ? Dial 1: Set to 30 for <= 10^9. Set to 60 for <= 10^18.
    const int BITS = 30;

    struct Node
    {
        int child[2];
        int cnt; // How many elements exist in this subtree

        Node()
        {
            child[0] = child[1] = 0;
            cnt = 0;
        }
    };

    vector<Node> trie;

    // ? Creates a brand new node that is an exact copy of the given node ID
    int clone(int node)
    {
        trie.push_back(trie[node]);
        return trie.size() - 1;
    }

    int insert(int prev_root, long long val)
    {
        int curr = clone(prev_root);
        int new_root = curr;
        int prev = prev_root;

        trie[curr].cnt++; // Increment count for the root of this version

        for (int i = BITS; i >= 0; i--)
        {
            int b = (val >> i) & 1;

            // ? We are taking path 'b'. To safely modify it without destroying
            // the historical version, we MUST clone the 'b' child of the previous version!
            trie[curr].child[b] = clone(trie[prev].child[b]);

            // The OTHER child pointer (b ^ 1) was naturally copied over during the
            // clone() of the parent, so it still safely points to the historical branch!

            curr = trie[curr].child[b];
            prev = trie[prev].child[b];

            trie[curr].cnt++; // Increment the count of elements that pass through this path
        }

        return new_root;
    }

public:
    // roots[i] represents the Trie containing all elements from index 0 to i-1.
    // roots[0] is the empty Trie.
    vector<int> roots;

    PersistentTrie(const vector<long long> &a = {})
    {
        // Pre-allocate memory to prevent slow vector reallocations.
        // N elements * roughly (BITS + 1) nodes per element
        trie.reserve(a.size() * (BITS + 2) + 100);

        // Node 0 is the Dummy/Empty node
        trie.emplace_back();

        // Version 0 is the empty tree
        roots.push_back(0);

        for (long long x : a)
        {
            insertBack(x);
        }
    }

    // Add an element to the end of the array dynamically
    void insertBack(long long val)
    {
        int new_root = insert(roots.back(), val);
        roots.push_back(new_root);
    }

    // Find the MAXIMUM value of (val ^ Y) where Y is an element in a[l...r]
    long long queryMaxXor(int l, int r, long long val)
    {
        // Because roots[i] covers [0...i-1], the range [L, R] is extracted by
        // taking Version (R + 1) and subtracting Version L.
        int curr_R = roots[r + 1];
        int curr_L = roots[l];
        long long ans = 0;

        for (int i = BITS; i >= 0; i--)
        {
            int b = (val >> i) & 1;
            int flipped = b ^ 1; // To MAXIMIZE XOR, we want the opposite bit!

            // How many numbers in our specific range actually take the 'flipped' path?
            int count_flipped = trie[trie[curr_R].child[flipped]].cnt -
                                trie[trie[curr_L].child[flipped]].cnt;

            if (count_flipped > 0)
            {
                // The path exists! We take it.
                ans |= (1LL << i);
                curr_R = trie[curr_R].child[flipped];
                curr_L = trie[curr_L].child[flipped];
            }
            else
            {
                // The opposite bit doesn't exist in this range. We are forced to take the same bit.
                curr_R = trie[curr_R].child[b];
                curr_L = trie[curr_L].child[b];
            }
        }
        return ans;
    }

    // Find the MINIMUM value of (val ^ Y) where Y is an element in a[l...r]
    long long queryMinXor(int l, int r, long long val)
    {
        int curr_R = roots[r + 1];
        int curr_L = roots[l];
        long long ans = 0;

        for (int i = BITS; i >= 0; i--)
        {
            int b = (val >> i) & 1;

            // To MINIMIZE XOR, we want the EXACT SAME bit!
            int count_same = trie[trie[curr_R].child[b]].cnt -
                             trie[trie[curr_L].child[b]].cnt;

            if (count_same > 0)
            {
                // The path exists! We take it. (Ans bit remains 0 because b ^ b = 0)
                curr_R = trie[curr_R].child[b];
                curr_L = trie[curr_L].child[b];
            }
            else
            {
                // Forced to take the flipped bit, which increases our XOR penalty.
                int flipped = b ^ 1;
                ans |= (1LL << i);
                curr_R = trie[curr_R].child[flipped];
                curr_L = trie[curr_L].child[flipped];
            }
        }
        return ans;
    }
};

void solve()
{
    int n, q;
    if (!(cin >> n >> q))
        return;

    vector<long long> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    PersistentTrie ptrie(a);

    while (q--)
    {
        int l, r;
        long long x;
        // 0-indexed L and R, and the XOR target X
        cin >> l >> r >> x;

        cout << ptrie.queryMaxXor(l, r, x) << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}