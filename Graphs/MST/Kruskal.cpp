class DSU
{
private:
    vector<int> parent, rank;
    int forests{};

    void link(int x, int y)
    {
        if (rank[x] > rank[y])
            swap(x, y);
        parent[x] = y;
        if (rank[x] == rank[y])
            rank[y]++;
    }

public:
    DSU(int n)
    {
        parent = vector<int>(n), rank = vector<int>(n);
        forests = n;
        for (int i = 0; i < n; i++)
            parent[i] = i, rank[i] = 1;
    }

    int find_set(int x)
    {
        if (x == parent[x])
            return x;
        return parent[x] = find_set(parent[x]);
    }

    int union_sets(int x, int y)
    {
        x = find_set(x), y = find_set(y);
        if (x != y)
        {
            link(x, y);
            forests--;
        }
        return x != y;
    }
};

struct edge
{
    int from, to, w;

    edge(int from, int to, int w) : from(from), to(to), w(w)
    {
    }

    bool operator<(const edge &e) const
    {
        return w < e.w;
    }
};

int Kruskal(vector<edge> &edgeList, int n)
{
    DSU uf(n + 1);
    vector<edge> edges;
    int mnCost{};
    sort(edgeList.begin(), edgeList.end());
    for (auto &[from, to, cost] : edgeList)
    {
        if (uf.union_sets(from, to))
        {
            edges.emplace_back(from, to, cost);
            mnCost += cost;
        }
    }
    return mnCost;
}
