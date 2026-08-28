/// if we need to calculate (n % m) if  EX: n = 10^10000 and m = 10^9
ll Big_Mod_Power(string n, ll m) 
{
    ll x = 0;
    for(ll i = 0; i < n.size(); i++)
    {
        x = ((x * 10)) + (n[i] - '0');
        x %= m;   
    }
    return x;  /// return n % m
}
