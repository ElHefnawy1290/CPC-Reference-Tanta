#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DINIC'S ALGORITHM
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Max Flow", "Min-Cut", "Bipartite Matching", "Maximum Closure".
 * - Classic Scenarios: You have a network of pipes with capacities and need to find
 *   the maximum amount of "stuff" (flow) that can move from a Source to a Sink.
 * - The Magic:
 *     a) BFS: Builds a Level Graph (only allows flow from distance d to d+1).
 *     b) DFS: Pushes flow along the Level Graph.
 *     c) ptr: The "pointer optimization" ensures we don't re-scan edges that are
 *        already saturated in the current DFS phase (the secret to O(V^2 E)).
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       Dinic flow(N);
 * - Add Edge:
 *       flow.add_edge(u, v, capacity);
 * - Query:
 *       long long max_f = flow.max_flow(source, sink);
 *
 * - Complexity:
 *       General: O(V^2 * E).
 *       Unit Networks (e.g., Bipartite Matching): O(E * sqrt(V)).
 */

struct Dinic
{
    struct Edge
    {
        int to;
        long long cap, flow;
        int rev; // Index of the reverse edge in adj[to]
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, long long cap)
    {
        // Forward edge: capacity 'cap', flow 0
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        // Backward edge: capacity 0, flow 0
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
    }

    // BFS to build the Level Graph
    bool bfs(int s, int t)
    {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            for (auto &edge : adj[v])
            {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1)
                {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    // DFS to push flow through the Level Graph
    long long dfs(int v, int t, long long pushed)
    {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;

        // ? Dial 1: The Pointer Optimization
        // ptr[v] ensures we don't look at edges that we already know are dead ends
        for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++)
        {
            auto &edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0)
                continue;

            long long tr_pushed = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (tr_pushed == 0)
                continue;

            edge.flow += tr_pushed;
            adj[tr][edge.rev].flow -= tr_pushed;
            return tr_pushed;
        }
        return 0;
    }

    long long max_flow(int s, int t)
    {
        long long flow = 0;
        while (bfs(s, t))
        {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, 1e18))
            {
                flow += pushed;
            }
        }
        return flow;
    }
};