int LIS(int i)
{
    if (i > n)
        return 0;
    auto &ret = dp[i];
    if (dp[i] == -1)
        return ret;
    ret = 0;
    for (int j = i + 1; j <= n; j++)
    {
        if (v[j] > v[i])
            ret = max(ret, LIS(j));
    }
    ret++;
    return ret;
}

// int res{};
// for (int i = 1; i <= n; i++)
// {
//     int num = LIS(i);
//     res = max(num, res);
// }
