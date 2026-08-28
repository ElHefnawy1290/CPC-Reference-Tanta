struct SqrtRangeUpdateRangeQuery
{
    int n, S;
    vector<long long> a, b, c; // b = lazy, c = sum of block

    SqrtRangeUpdateRangeQuery(int n, const vector<long long> &arr)
    {
        this->n = n;
        S = ceil(sqrt(n));
        a = arr;
        b.assign(S + 1, 0);
        c.assign(S + 1, 0);
        for (int i = 0; i < n; ++i)
        {
            c[i / S] += a[i];
        }
    }

    // Helper to get block actual sum safely
    long long get_block_sum(int k)
    {
        // Block sum = original sum + (lazy value * number of elements in block)
        long long elements_count = min(n - k * S, S);
        return c[k] + b[k] * elements_count;
    }

    // O(sqrt(N))
    void add(int l, int r, long long delta)
    {
        int c_l = l / S, c_r = r / S;

        if (c_l == c_r)
        {
            for (int i = l; i <= r; ++i)
            {
                a[i] += delta;
                c[c_l] += delta;
            }
        }
        else
        {
            for (int i = l, end = (c_l + 1) * S - 1; i <= end; ++i)
            {
                a[i] += delta;
                c[c_l] += delta;
            }
            for (int k = c_l + 1; k <= c_r - 1; ++k)
            {
                b[k] += delta; // Lazy update
            }
            for (int i = c_r * S; i <= r; ++i)
            {
                a[i] += delta;
                c[c_r] += delta;
            }
        }
    }

    // O(sqrt(N))
    long long query(int l, int r)
    {
        long long sum = 0;
        int c_l = l / S, c_r = r / S;

        if (c_l == c_r)
        {
            for (int i = l; i <= r; ++i)
                sum += a[i] + b[c_l];
        }
        else
        {
            for (int i = l, end = (c_l + 1) * S - 1; i <= end; ++i)
                sum += a[i] + b[c_l];

            for (int k = c_l + 1; k <= c_r - 1; ++k)
                sum += get_block_sum(k); // Full block sum

            for (int i = c_r * S; i <= r; ++i)
                sum += a[i] + b[c_r];
        }
        return sum;
    }
};