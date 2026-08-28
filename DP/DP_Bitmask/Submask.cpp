int dp[1<<16];
int cost[1<<16];
int memo (int mask) {
    if (mask == 0) return 0;
    auto &ret = dp[mask];
    if (~ret) return ret;
    for (int i = mask;i > 0;i = (i-1) & mask) {
        ret = max(ret,cost[i] + memo(mask ^ i));
    }
    return ret;
}
void hassan() {
    int n;cin >>n;
    int a[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
        }
    }
    for (int mask = 0; mask < (1<<n); mask++) {
        for (int i = 0; i < n; i++) {
            if (mask & (1<<i)) {
                for (int j = 0; j < i; j++) {
                    if (mask & (1<<j)) {
                        cost[mask]+=a[i][j];
                    }
                }
            }
        }
      //  cout << mask << " >> " << cost[mask] << endl;
    }
    memset(dp, -1, sizeof(dp));
    cout << memo((1<<n)-1);

}
