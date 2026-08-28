struct edge
{
    ll from, to, w;

    edge(ll from, ll to, ll w) : from(from), to(to), w(w)
    {
    }

    bool operator<(const edge &e) const
    {
        return w < e.w;
    }

    bool operator==(const edge &e)
    {
        return from == e.from && to == e.to && w == e.w;
    }
};
