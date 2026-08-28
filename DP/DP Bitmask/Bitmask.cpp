int n;
vector<vector<int>> v;
int dp[22][1 << 22];

int sol(int man, int mask)
{
    if (man >= n)
        return 1;
    auto &ret = dp[man][mask];
    if (~ret)
        return ret;
    ret = 0;
    for (int i = 0; i < n; i++)
    {
        if (mask >> i & 1)
            continue;
        if (v[man][i])
        {
            ret += sol(man + 1, mask | 1 << i);
            ret %= MOD;
        }
    }
    return ret;
}
