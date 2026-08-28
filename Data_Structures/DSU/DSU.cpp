class DSU
{
private:
    vector<int> parent, size;
    int forests{};

    void link(int x, int y)
    {
        if (size[x] > size[y])
            swap(x, y);
        parent[x] = y;
        size[y] += size[x];
    }

public:
    DSU(int n)
    {
        parent = vector<int>(n), size = vector<int>(n);
        forests = n;
        for (int i = 0; i < n; i++)
            parent[i] = i, size[i] = 1;
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

    bool same_set(int x, int y)
    {
        x = find_set(x), y = find_set(y);
        return x == y;
    }

    int get_size(int x)
    {
        return size[find_set(x)];
    }

    int get_forests() const
    {
        return forests;
    }
};
