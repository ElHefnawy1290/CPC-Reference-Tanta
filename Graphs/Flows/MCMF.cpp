#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: MIN COST MAX FLOW (MCMF)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Cheapest Flow", "Matching with Costs", "Assignment Problem".
 * - Classic Scenarios: You have workers and tasks with different costs. You need
 *   to assign every worker a task such that the total cost is minimized.
 * - The Magic: It works by finding the "shortest" path (where "shortest" is defined by
 *   edge cost) in the residual graph. It keeps augmenting flow along these shortest
 *   paths until no path from source to sink exists.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       MCMF solver(N);
 * - Add Edge:
 *       solver.add_edge(u, v, capacity, cost);
 * - Query:
 *       pair<long long, long long> result = solver.min_cost_max_flow(source, sink);
 *       // result.first = max flow, result.second = min cost
 *
 * - Complexity:
 *       O(F * E * log V) using Dijkstra + Potentials.
 *       O(F * E * V) using SPFA (included below for simplicity with negative edges).
 */

const long long INF = 1e18;

struct Edge
{
    int to;
    long long cap, flow, cost;
    int rev; // Reverse edge index
};

struct MCMF
{
    int n;
    vector<vector<Edge>> adj;
    vector<long long> dist, parent_edge, parent_node;

    MCMF(int n) : n(n), adj(n), dist(n), parent_edge(n), parent_node(n) {}

    void add_edge(int from, int to, long long cap, long long cost)
    {
        adj[from].push_back({to, cap, 0, cost, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, -cost, (int)adj[from].size() - 1});
    }

    // SPFA is used here to handle negative costs.
    // If all costs are positive, Dijkstra with potentials is faster.
    bool spfa(int s, int t, long long &flow, long long &cost)
    {
        dist.assign(n, INF);
        parent_node.assign(n, -1);
        parent_edge.assign(n, -1);
        vector<bool> in_queue(n, false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        in_queue[s] = true;

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            in_queue[u] = false;

            for (int i = 0; i < (int)adj[u].size(); i++)
            {
                Edge &e = adj[u][i];
                if (e.cap - e.flow > 0 && dist[e.to] > dist[u] + e.cost)
                {
                    dist[e.to] = dist[u] + e.cost;
                    parent_node[e.to] = u;
                    parent_edge[e.to] = i;
                    if (!in_queue[e.to])
                    {
                        q.push(e.to);
                        in_queue[e.to] = true;
                    }
                }
            }
        }

        if (dist[t] == INF)
            return false;

        // Push flow
        long long push = INF;
        int curr = t;
        while (curr != s)
        {
            int prev = parent_node[curr];
            int idx = parent_edge[curr];
            push = min(push, adj[prev][idx].cap - adj[prev][idx].flow);
            curr = prev;
        }

        flow += push;
        cost += push * dist[t];
        curr = t;
        while (curr != s)
        {
            int prev = parent_node[curr];
            int idx = parent_edge[curr];
            adj[prev][idx].flow += push;
            int rev_idx = adj[prev][idx].rev;
            adj[curr][rev_idx].flow -= push;
            curr = prev;
        }

        return true;
    }

    pair<long long, long long> min_cost_max_flow(int s, int t)
    {
        long long flow = 0, cost = 0;
        while (spfa(s, t, flow, cost))
            ;
        return {flow, cost};
    }
};