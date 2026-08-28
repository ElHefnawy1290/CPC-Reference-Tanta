int nCr(int n, int r)
{
    if (r < 0 || n < 0 || r > n)
        return 0;
    int ret{1};
    for (int i = 1; i <= min(r, n - r); i++)
    {
        ret *= n - i + 1;
        ret /= i;
    }
    return ret;
}

int nPr(int n, int r)
{
    if (r < 0 || n < 0 || r > n)
        return 0;
    int ret{1};
    for (int i = 0; i < r; i++)
        ret = mul(ret, n - i);
    return ret;
}
