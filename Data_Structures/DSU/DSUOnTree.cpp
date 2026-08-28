/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DSU ON TREE (SACK / SMALL TO LARGE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Subtree queries", "Offline queries", "Distinct colors in subtree", "Frequencies at depth".
 * - Classic Scenarios: You have a tree and Q offline queries asking about properties within 
 *   the subtree of node U (e.g., "Can we form a palindrome using nodes at depth H in U's subtree?").
 *   A naive DFS for each query gives TLE O(N^2).
 * - The Magic: We use "Small to Large" merging. We find the "heavy" child (largest subtree). 
 *   We process light children and ERASE their data, then process the heavy child and KEEP its data. 
 *   Finally, we add light children back. Since a node is a "light child" at most O(log N) times, 
 *   the complexity drops to O(N log N).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass the number of nodes, the string of characters, and number of queries.
 *       DSUOnTree dsu_tree(N, s, Q);
 *
 * - Updates: Add edges to build the tree, and add queries offline.
 *       dsu_tree.addEdge(u, v);
 *       dsu_tree.addQuery(node, depth, query_index);
 *
 * - Execution: Call solve(root) to process everything and get the answers.
 *       vector<string> answers = dsu_tree.process(1);
 *
 * - Complexity:
 *       Time: O(N log N) processing time.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 0-indexed or 1-indexed?
 *   The template naturally handles 1-indexed nodes. If your string is 0-indexed, it automatically 
 *   shifts it to 1-indexed internally for safety.
 * - Changing the problem condition:
 *   Modify the `modifyState()` function to count frequencies, update maximums, or manage a `std::set`.
 *   Then modify the "Answer queries" section in `dfsDSU()` to check your specific condition.
 */

#include <bits/stdc++.h>
using namespace std;

struct DSUOnTree
{
private:
    int n;
    string s;
    vector<vector<int>> adj;
    vector<vector<pair<int, int>>> queries; // {depth, query_index}
    
    vector<int> sz;
    vector<int> lvl;
    vector<int> mask; // Bitmask to track odd/even frequencies of characters
    vector<bool> big; // Marks the heavy child
    vector<string> ans;

    // Step 1: Precalculate subtree sizes and depths
    void dfsSize(int node, int parent, int level)
    {
        lvl[node] = level;
        sz[node] = 1;
        for (int child : adj[node])
        {
            if (child != parent)
            {
                dfsSize(child, node, level + 1);
                sz[node] += sz[child];
            }
        }
    }

    // Step 2: Add or remove a node's contribution from our global state
    void modifyState(int node, int parent, int x)
    {
        // ? THE MAGIC: We toggle the i-th bit to flip the parity of the character's frequency
        mask[lvl[node]] ^= (1 << (s[node] - 'a'));

        for (int child : adj[node])
        {
            // ? CRITICAL: Do NOT traverse the heavy child if it's already marked!
            if (child != parent && !big[child])
            {
                modifyState(child, node, x);
            }
        }
    }

    // Step 3: The core DSU on Tree logic
    void dfsDSU(int node, int parent, bool keep)
    {
        int maxSize = -1, bigChild = -1;
        
        // Find the heavy child
        for (int child : adj[node])
        {
            if (child != parent && sz[child] > maxSize)
            {
                maxSize = sz[child];
                bigChild = child;
            }
        }

        // Process all light children first, and ERASE their data (keep = 0)
        for (int child : adj[node])
        {
            if (child != parent && child != bigChild)
            {
                dfsDSU(child, node, 0);
            }
        }

        // Process the heavy child, and KEEP its data (keep = 1)
        if (bigChild != -1)
        {
            dfsDSU(bigChild, node, 1);
            big[bigChild] = true; // Mark as heavy so `modifyState` ignores it
        }

        // Add the current node and its light children back to the state
        modifyState(node, parent, 1);

        // --- ANSWER QUERIES FOR THIS SUBTREE ---
        for (auto& q : queries[node])
        {
            int target_depth = q.first;
            int query_id = q.second;

            // ? THE MAGIC: If at most 1 bit is set, the characters can form a palindrome.
            if (__builtin_popcount(mask[target_depth]) <= 1)
            {
                ans[query_id] = "Yes";
            }
            else
            {
                ans[query_id] = "No";
            }
        }

        // Cleanup: Unmark the heavy child
        if (bigChild != -1)
        {
            big[bigChild] = false;
        }

        // ? CRITICAL: If this whole subtree was a light child, erase it completely!
        if (keep == 0)
        {
            modifyState(node, parent, -1);
        }
    }

public:
    DSUOnTree(int n, string input_s, int q_count)
    {
        this->n = n;
        // Pad the string so it becomes 1-indexed to match the nodes
        this->s = " " + input_s; 
        
        adj.resize(n + 1);
        queries.resize(n + 1);
        sz.assign(n + 1, 0);
        lvl.assign(n + 1, 0);
        mask.assign(n + 1, 0);
        big.assign(n + 1, false);
        ans.resize(q_count);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void addQuery(int node, int depth, int query_index)
    {
        queries[node].push_back({depth, query_index});
    }

    vector<string> process(int root = 1)
    {
        dfsSize(root, 0, 1);
        dfsDSU(root, 0, 0);
        return ans;
    }
};

void solve()
{
    int n, q;
    // Example Input: N nodes, Q queries
    if (!(cin >> n >> q)) return;

    string s;
    cin >> s;

    DSUOnTree dsu_tree(n, s, q);

    for (int i = 1; i < n; i++)
    {
        int u, v;
        cin >> u >> v;
        dsu_tree.addEdge(u, v);
    }

    for (int i = 0; i < q; i++)
    {
        int v, h;
        cin >> v >> h;
        // 0-indexed queries
        dsu_tree.addQuery(v, h, i); 
    }

    // Process all queries offline
    vector<string> results = dsu_tree.process(1);

    for (const string& res : results)
    {
        cout << res << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}
