/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: EULERIAN PATH / CIRCUIT (HIERHOLZER'S ALGORITHM)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Visit every edge exactly once", "Eulerian Tour", "Draw without lifting pen", "De Bruijn Sequence".
 * - Classic Scenarios: You are given a graph (directed or undirected) and you need to find a 
 *   path or cycle that traverses EVERY edge exactly once. Standard DFS/BFS won't work because 
 *   they visit *nodes* once, but here we care about *edges*.
 * - The Magic: "Hierholzer's Algorithm". We do a greedy DFS, destroying edges as we walk over them 
 *   using `.pop_back()` (to ensure we never process an edge twice, keeping time complexity strictly O(E)). 
 *   When we hit a "dead end" (a node with no outgoing edges left), we add it to our answer array. 
 *   Because of the mathematical properties of Eulerian graphs, this backtracking naturally constructs 
 *   the exact path in REVERSE!
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Clear the `vis` array and the `ans` vector. 
 * - Build Graph: For every edge, assign it a unique `id` from 0 to M-1.
 *       graph[u].push_back({v, id});
 *       // If undirected, add the reverse edge with the SAME id!
 *       // graph[v].push_back({u, id}); 
 *
 * - Execution: Call `dfs(start_node)`. 
 *       dfs(start_node);
 *       reverse(ans.begin(), ans.end()); // ? CRITICAL: The path is generated backwards!
 *
 * - Complexity:
 *       Time: O(V + E) — Each edge is popped and visited exactly once.
 *       Space: O(V + E) for the adjacency list and recursion stack.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - ⚠️ CRITICAL PREREQUISITE (EXISTENCE CHECK): This DFS assumes the Eulerian path ALREADY EXISTS. 
 *   If you run it on a random graph, it will produce garbage. You MUST check the degrees first!
 *   - Undirected Circuit: All nodes must have an EVEN degree.
 *   - Undirected Path: Exactly 0 or 2 nodes must have an ODD degree. (Start DFS from an odd node!).
 *   - Directed Circuit: Every node's In-Degree == Out-Degree.
 *   - Directed Path: At most one node has (Out - In == 1) [Start Node], and one has (In - Out == 1) [End Node].
 * - Lexicographically Smallest Path?
 *   If the problem asks for the lexicographically smallest path, sort the adjacency list of every 
 *   node in DESCENDING order before starting the DFS. Since we use `.pop_back()`, the smallest 
 *   destinations will be popped first!
 * - Disconnected components: If the graph has isolated edges, a single DFS won't catch them. 
 *   Always check if `ans.size() == M + 1` at the end to ensure all edges were visited.
 */
const int N = 1e5, M = 1e5;

struct Edge
{
    int to, id;
};

vector<Edge> graph[N];
bool vis[M];
vector<int> ans;

void dfs(int u)
{
    while ((int)graph[u].size() > 0)
    {
        auto [v, id] = graph[u].back();
        graph[u].pop_back();
        if (!vis[id])
        {
            vis[id] = true;
            dfs(v);
        }
    }
    ans.emplace_back(u); // Eulerian path reversed
}
