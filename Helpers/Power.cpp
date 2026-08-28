ll power(ll base, ll pow)
{
    base %= MOD;
    ll res{1};
    while (pow)
    {
        if (pow & 1)
            res = res * base % MOD;
        base = base * base % MOD;
        pow >>= 1;
    }
    return res;
}
