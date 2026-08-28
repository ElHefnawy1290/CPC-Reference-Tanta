#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: TREE ISOMORPHISM (HASHING)
 *
 * Solves: Checks if two unrooted trees are structurally identical.
 * Time Complexity: O(N)
 * Space Complexity: O(N)
 */

struct TreeIsomorphism
{
    // Generate a random seed once per program execution to prevent anti-hash tests
    static uint64_t get_seed()
    {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        return rng();
    }

    // A highly robust non-linear permutation function (SplitMix64)
    static uint64_t shift_hash(uint64_t x)
    {
        static const uint64_t SEED = get_seed();
        x += SEED;
        x ^= (x >> 33);
        x *= 0xff51afd7ed558ccdULL;
        x ^= (x >> 33);
        x *= 0xc4ceb9fe1a85ec53ULL;
        x ^= (x >> 33);
        return x;
    }

    static vector<int> get_centers(int n, const vector<vector<int>> &adj)
    {
        vector<int> degree(n + 1);
        queue<int> q;

        for (int i = 1; i <= n; i++)
        {
            degree[i] = adj[i].size();
            // Leaves have degree 1 (or 0 if n=1)
            if (degree[i] <= 1)
            {
                q.push(i);
            }
        }

        int remaining_nodes = n;

        // Peel away the leaves layer by layer like an onion
        while (remaining_nodes > 2)
        {
            int layer_size = q.size();
            remaining_nodes -= layer_size;

            for (int i = 0; i < layer_size; i++)
            {
                int u = q.front();
                q.pop();

                for (int v : adj[u])
                {
                    if (--degree[v] == 1)
                    {
                        q.push(v);
                    }
                }
            }
        }

        // The remaining 1 or 2 nodes are the absolute centers
        vector<int> centers;
        while (!q.empty())
        {
            centers.push_back(q.front());
            q.pop();
        }
        return centers;
    }

    static uint64_t compute_hash(int u, int p, const vector<vector<int>> &adj)
    {
        uint64_t current_hash = 1; // Base value for a node

        for (int v : adj[u])
        {
            if (v != p)
            {
                // To combine children, we hash the child's subtree, run it through
                // our non-linear shift function, and sum them up.
                // Using SUM guarantees that the order of children does not matter!
                current_hash += shift_hash(compute_hash(v, u, adj));
            }
        }
        return current_hash;
    }

    static bool are_isomorphic(int n, const vector<vector<int>> &adj1, const vector<vector<int>> &adj2)
    {
        if (n == 1)
            return true; // Trivial case

        // 1. Find centers of both trees
        vector<int> centers1 = get_centers(n, adj1);
        vector<int> centers2 = get_centers(n, adj2);

        // If the number of centers doesn't match, they physically cannot be isomorphic
        if (centers1.size() != centers2.size())
        {
            return false;
        }

        // 2. Hash Tree 1 from its first center
        uint64_t hash1 = compute_hash(centers1[0], 0, adj1);

        // 3. Check Tree 2 against Tree 1's hash
        for (int c2 : centers2)
        {
            uint64_t hash2 = compute_hash(c2, 0, adj2);
            if (hash1 == hash2)
            {
                return true; // Match found!
            }
        }

        return false;
    }
};

void solve()
{
    int n;
    // Example: Read N, then N-1 edges for Tree 1, then N-1 edges for Tree 2
    if (!(cin >> n))
        return;

    vector<vector<int>> tree1(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        tree1[u].push_back(v);
        tree1[v].push_back(u);
    }

    vector<vector<int>> tree2(n + 1);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        cin >> u >> v;
        tree2[u].push_back(v);
        tree2[v].push_back(u);
    }

    if (TreeIsomorphism::are_isomorphic(n, tree1, tree2))
    {
        cout << "Isomorphic (They are the exact same tree)\n";
    }
    else
    {
        cout << "Not Isomorphic\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}