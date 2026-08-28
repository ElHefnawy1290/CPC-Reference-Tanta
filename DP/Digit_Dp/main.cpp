string s;
int d, k;
int dp[20][20][2][2];

int sol(int idx, int cnt, int tight, int num)
{
    if (idx >= int(s.size()))
    {
        if (cnt == k)
            return 1;
        return 0;
    }
    auto &ret = dp[idx][cnt][tight][num];
    if (~ret)
        return ret;
    ret = 0;
    int limit = tight ? s[idx] - '0' : 9;
    for (int i = 0; i <= limit; i++)
    {
        int nn = num || i;
        int nt = tight && i == limit;
        int nc = cnt + (d == i && nn);
        ret += sol(idx + 1, nc, nt, nn);
    }
    return ret;
}

void solve()
{
    cin >> s >> d >> k;
    memset(dp, -1, sizeof dp);
    cout << sol(0, 0, 1, 0);
}
