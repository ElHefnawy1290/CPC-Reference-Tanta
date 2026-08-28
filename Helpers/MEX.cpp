int MEX(vector<int> &v)
{
    int n = int(v.size());
    vector<int> frq(n);
    for (int i = 0; i < n; i++)
    {
        if (v[i] < n)
            frq[v[i]]++;
    }
    for (int i = 0; i < n; i++)
    {
        if (frq[i] == 0)
            return i;
    }
    return n;
}
