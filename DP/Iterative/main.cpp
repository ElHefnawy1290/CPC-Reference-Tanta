int answer(int n)
{
    if (n == 0 || n == 1)
        return n;
    return dp[n];
}

void solve()
{
    int n;
    cin >> n;
    for (int i = 2; i <= n; i++)
        dp[i] = answer(i - 1) + answer(i - 2);
    cout << answer(n);
}
