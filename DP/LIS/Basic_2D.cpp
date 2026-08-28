int dp1[2010][2010];
int dp2[2010][2010];

int LDS(int i, int prev)
{
    if (i > n)
        return 0;
    auto &ret = dp1[i][prev];
    if (~ret)
        return ret;
    ret = LDS(i + 1, prev);
    if (prev == 0 || a[i] < a[prev])
        ret = max(ret, 1 + LDS(i + 1, i));
    return ret;
}

int LIS(int i, int prev)
{
    if (i > n)
        return 0;
    auto &ret = dp2[i][prev];
    if (~ret)
        return ret;
    ret = LIS(i + 1, prev);
    if (prev == 0 || a[i] > a[prev])
        ret = max(ret, 1 + LIS(i + 1, i));
    return ret;
}
