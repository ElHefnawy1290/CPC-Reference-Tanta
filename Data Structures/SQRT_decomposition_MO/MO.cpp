
// how many distinct values are there in a range [l,r]

int SQ = 400;
struct Query
{
    int l, r, idx;
    bool operator<(const Query &q) const
    {
        if (l / SQ != q.l / SQ)
        {
            return l / SQ < q.l / SQ;
        }
        return (l / SQ & 1) ? r < q.r : r > q.r;
    }
};

void solve()
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n), frq(n + 10);
    map<int, int> mp;
    int id{};
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        if (!mp.count(a[i]))
        {
            mp[a[i]] = id++;
        }
    }
    for (int i = 0; i < n; i++)
        a[i] = mp[a[i]];

    vector<Query> query(q);
    for (int i = 0; i < q; i++)
    {
        cin >> query[i].l >> query[i].r;
        query[i].l--, query[i].r--;
        query[i].idx = i;
    }
    sort(query.begin(), query.end());
    int ans = 0;
    auto add = [&](int i)
    {
        // if (frq[a[i]] == 0)
        // {
        //     ans++;
        // }
        // frq[a[i]]++;
    };
    auto remove = [&](int i)
    {
        // frq[a[i]]--;
        // if (frq[a[i]] == 0)
        // {
        //     ans--;
        // }
    };

    vector<int> res(q);
    int l = 0, r = -1;
    for (auto [ql, qr, idx] : query)
    {
        while (l > ql)
        {
            --l;
            add(l);
        }
        while (r < qr)
        {
            ++r;
            add(r);
        }
        while (l < ql)
        {
            remove(l);
            ++l;
        }
        while (r > qr)
        {
            remove(r);
            --r;
        }
        res[idx] = ans;
    }
    for (int i = 0; i < q; i++)
        cout << res[i] << "\n";
}