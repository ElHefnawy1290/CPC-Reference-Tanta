struct Query
{
    int x, a, id;

    bool operator<(const Query &other) const
    {
        return a < other.a;
    }
};
