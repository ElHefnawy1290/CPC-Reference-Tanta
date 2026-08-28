#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DEGREE SEQUENCES
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Given an array of degrees", "Can we form a simple graph", "Construct a graph".
 * - Classic Scenarios: You are given N integers representing the number of friends each person has.
 *   You need to verify if this network is mathematically possible, and if so, pair them up.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Validation Only (Fast):
 *       bool possible = DegreeSequence::erdos_gallai(degrees);
 *       // Complexity: O(N log N). Use this when N = 10^5 and you only need YES/NO.
 *
 * - Construction (Slower, but gives edges):
 *       vector<pair<int, int>> edges = DegreeSequence::havel_hakimi(degrees);
 *       // Complexity: O(N^2 log N). Use this when you must PRINT the graph.
 *       // Returns an empty vector if construction is impossible.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 0-based vs 1-based indexing for Construction:
 *   The `havel_hakimi` function returns edges using 1-based indexing by default.
 *   If you need 0-based indexing, change `nodes.push_back({d[i], i + 1});` to `i`.
 */

struct DegreeSequence
{

    static bool erdos_gallai(vector<long long> d)
    {
        int n = d.size();
        long long sum = 0;

        // A valid graph MUST have an even sum of degrees (Handshaking Lemma)
        for (long long x : d)
            sum += x;
        if (sum % 2 != 0)
            return false;

        // Sort degrees in descending order: d_1 >= d_2 >= ... >= d_n
        sort(d.begin(), d.end(), greater<long long>());

        // Build 1-indexed prefix sums for O(1) range queries
        vector<long long> pref(n + 1, 0);
        for (int i = 0; i < n; i++)
        {
            pref[i + 1] = pref[i] + d[i];
        }

        // Evaluate the Erdős-Gallai inequality for every k from 1 to n:
        // Sum(i=1 to k) d_i <= k(k-1) + Sum(i=k+1 to n) min(d_i, k)
        for (int k = 1; k <= n; k++)
        {
            long long left_sum = pref[k];

            // Binary search to find the split point in the suffix where d_i drops below k.
            // This avoids an O(N) loop and keeps the overall complexity at O(N log N).
            int p = max(k, (int)(lower_bound(d.begin(), d.end(), k, greater<long long>()) - d.begin()));

            long long right_sum = 1LL * k * (k - 1);

            // Elements from k to p-1 are >= k, so min(d_i, k) is always exactly k
            right_sum += 1LL * k * (p - k);

            // Elements from p to n-1 are < k, so min(d_i, k) is just d_i
            right_sum += (pref[n] - pref[p]);

            if (left_sum > right_sum)
            {
                return false; // Mathematical contradiction found!
            }
        }

        return true;
    }

    static vector<pair<int, int>> havel_hakimi(const vector<int> &d)
    {
        int n = d.size();
        long long sum = 0;
        for (int x : d)
            sum += x;
        if (sum % 2 != 0)
            return {}; // Fails Handshaking Lemma

        // Store {degree, original_node_id}
        vector<pair<int, int>> nodes;
        for (int i = 0; i < n; i++)
        {
            if (d[i] > 0)
            {
                nodes.push_back({d[i], i + 1}); // 1-based indexing for nodes
            }
        }

        vector<pair<int, int>> edges;

        while (!nodes.empty())
        {
            // Sort descending by degree
            sort(nodes.begin(), nodes.end(), greater<pair<int, int>>());

            // The node with the highest degree demands to be connected
            int current_degree = nodes[0].first;
            int u = nodes[0].second;

            // Remove it from the pool of available nodes
            nodes.erase(nodes.begin());

            // If it demands more connections than there are nodes left, it's impossible
            if (current_degree > nodes.size())
                return {};

            // Connect 'u' greedily to the next 'current_degree' highest nodes
            for (int i = 0; i < current_degree; i++)
            {
                edges.push_back({u, nodes[i].second});
                nodes[i].first--; // They used up one of their connections

                // If a degree drops below 0, it means we forced a connection that broke the graph
                if (nodes[i].first < 0)
                    return {};
            }

            // Purge fully satisfied nodes (degree == 0) to speed up sorting
            vector<pair<int, int>> remaining_nodes;
            for (auto &node : nodes)
            {
                if (node.first > 0)
                {
                    remaining_nodes.push_back(node);
                }
            }
            nodes = remaining_nodes;
        }

        return edges;
    }
};

void solve()
{
    int n;
    // Example: Read N, then N degrees
    if (!(cin >> n))
        return;

    vector<int> d_int(n);
    vector<long long> d_ll(n);
    for (int i = 0; i < n; i++)
    {
        cin >> d_int[i];
        d_ll[i] = d_int[i];
    }

    // 1. O(N log N) Check (Best for large N)
    if (DegreeSequence::erdos_gallai(d_ll))
    {
        cout << "YES\n";

        // 2. O(N^2 log N) Construction (Only run if you actually need the edges!)
        // In CF/ICPC, if N > 10^4, they usually don't ask you to print the edges.
        vector<pair<int, int>> edges = DegreeSequence::havel_hakimi(d_int);
        for (auto &edge : edges)
        {
            cout << edge.first << " " << edge.second << "\n";
        }
    }
    else
    {
        cout << "NO\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve();
    return 0;
}