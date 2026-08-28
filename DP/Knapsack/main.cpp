int n, W;
int dp[1001][31];
vector<pair<int, int>> a;

int sol(int i, int w)
{
    if (i >= n)
        return 0;
    auto &ret = dp[i][w];
    if (~ret)
        return ret;
    ret = sol(i + 1, w);
    if (w + a[i].second <= W)
        ret = max(ret, a[i].first + sol(i + 1, w + a[i].second));
    return ret;
}
