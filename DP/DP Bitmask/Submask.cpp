ll dp[1 << 16], score[1 << 16];

ll sol(ll mask)
{
    if (mask == 0)
        return 0;
    auto &ret = dp[mask];
    if (~ret)
        return ret;
    ret = -OO;
    for (ll sub = mask; sub; sub = (sub - 1) & mask)
    {
        ll sum = score[sub];
        ret = max(ret, sum + sol(mask ^ sub));
    }
    return ret;
}
