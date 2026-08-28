vector<int> SOS(int n, vector<int> &a)
{
    vector<int> p = a;
    for (int dim = 0; dim < n; dim++)
    {
        for (int i = 0; i < 1 << n; i++)
        {
            if (i >> dim & 1)
                p[i] += p[i - (1 << dim)];
        }
    }
    return p;
}
