class DSU
{
private:
    vector<int> parent, size;
    stack<int> changes;
    int forests{};

    void link(int x, int y)
    {
        if (size[x] > size[y])
            swap(x, y);
        changes.push(x);
        parent[x] = y;
        size[y] += size[x];
    }

public:
    DSU(int n)
    {
        parent = vector<int>(n), size = vector<int>(n), changes = stack<int>();
        forests = n;
        for (int i = 0; i < n; i++)
            parent[i] = i, size[i] = 1;
    }

    int find_set(int x)
    {
        while (x != parent[x])
            x = parent[x];
        return x;
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

    void rollback()
    {
        if (changes.empty())
            return;
        int x = changes.top();
        changes.pop();
        size[parent[x]] -= size[x];
        parent[x] = x;
        forests++;
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
