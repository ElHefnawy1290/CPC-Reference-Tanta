template <typename T>
struct FenwickTree
{
    int n;
    vector<T> tree;

    // 1. Initialize an empty tree of size n (all zeros)
    FenwickTree(int n)
    {
        this->n = n;
        tree.assign(n + 1, 0);
    }

    // 2. Fast O(N) initialization from an existing array
    FenwickTree(const vector<T> &a)
    {
        n = a.size();
        tree.assign(n + 1, 0);

        // Step 1: Copy values in (shifting to 1-based indexing)
        for (int i = 0; i < n; i++)
        {
            tree[i + 1] = a[i];
        }

        // Step 2: Propagate the sums forward to the immediate parent
        for (int i = 1; i <= n; i++)
        {
            int parent = i + (i & -i);
            if (parent <= n)
            {
                tree[parent] += tree[i];
            }
        }
    }

    void add(int i, T delta)
    {
        for (; i <= n; i += (i & -i))
        {
            tree[i] += delta;
        }
    }

    T query(int i)
    {
        T sum = 0;
        for (; i > 0; i -= (i & -i))
        {
            sum += tree[i];
        }
        return sum;
    }

    T query(int l, int r)
    {
        return query(r) - query(l - 1);
    }
};
