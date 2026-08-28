/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MATROID INTERSECTION
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Spanning tree with unique colors", "Maximize intersection of rules".
 * - Classic Scenarios: Find the maximum number of elements from a set that satisfy
 *   TWO independent structural rules simultaneously (e.g., forms a forest AND has
 *   at most K edges from any specific set).
 * - The Magic: It builds a bipartite graph between the currently picked elements (IN)
 *   and unpicked elements (OUT). It searches for an augmenting path that swaps elements
 *   while maintaining independence in BOTH matroids.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Setup: You must define what "independence" means for your two rules by writing
 *   the `oracle1_can_add` and `oracle2_can_add` logic.
 * - Query:
 *       vector<int> independent_set = matroid_intersection(N);
 *
 * 3. ⚙️ ENGINEERING NOTE
 * - The robust, modular design here perfectly mirrors the isolation of concerns
 *   required in high-tier backend systems engineering. The core augmenting engine
 *   never cares about the underlying rules, only the true/false oracle responses.
 */

vector<int> matroid_intersection(int n)
{
    vector<bool> in_set(n, false);

    // Abstract augmenting path loop
    while (true)
    {
        // Build the bipartite exchange graph
        vector<int> X1, X2; // X1: valid starts, X2: valid ends
        vector<vector<int>> adj(n);

        // --- ORACLE 1 (e.g., Graphic Matroid) ---
        // For every out_node 'v', if {in_set U v} is independent, add 'v' to X1.
        // If dependent, find which in_node 'u' forms the circuit and add edge v -> u.

        // --- ORACLE 2 (e.g., Colorful Matroid) ---
        // For every out_node 'v', if {in_set U v} is independent, add 'v' to X2.
        // If dependent, find which in_node 'u' forms the circuit and add edge u -> v.

        // Example placeholders:
        /*
        for (int i = 0; i < n; i++) {
            if (!in_set[i]) {
                if (oracle1_can_add(i)) X1.push_back(i);
                if (oracle2_can_add(i)) X2.push_back(i);
            }
        }
        */

        // BFS to find the shortest augmenting path from X1 to X2
        queue<int> q;
        vector<int> dist(n, 1e9), parent(n, -1);

        for (int x : X1)
        {
            dist[x] = 0;
            q.push(x);
        }

        int end_node = -1;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();

            // Check if we hit the target set
            if (find(X2.begin(), X2.end(), u) != X2.end())
            {
                end_node = u;
                break;
            }

            for (int v : adj[u])
            {
                if (dist[v] > dist[u] + 1)
                {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        // If no augmenting path exists, we have reached the maximum independent set
        if (end_node == -1)
            break;

        // Augment the independent set by flipping states along the path
        int curr = end_node;
        while (curr != -1)
        {
            in_set[curr] = !in_set[curr];
            curr = parent[curr];
        }
    }

    vector<int> result;
    for (int i = 0; i < n; i++)
    {
        if (in_set[i])
            result.push_back(i);
    }
    return result;
}