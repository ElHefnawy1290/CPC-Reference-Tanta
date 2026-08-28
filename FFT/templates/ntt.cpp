


// This template implements the Number Theoretic Transform (NTT),
//  which is used to multiply two polynomials in $O(N \log N)$ time
//  modulo a specific prime. It also includes the building blocks for
//  Arbitrary Modulo
//  Convolution (MTT) using the Chinese Remainder Theorem (CRT).

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// Standard NTT-friendly prime (998244353) and its primitive root
const ll mod = (119 << 23) + 1, root = 62; 
// For p < 2^30 there is also e.g. 5 << 25, 7 << 26, 479 << 21
// and 483 << 21 (same root). The last two are > 10^9.

// Computes (b^e) % mod in O(log e) time
ll modpow(ll b, ll e) {
    ll ans = 1;
    for (; e; b = b * b % mod, e /= 2)
        if (e & 1) ans = ans * b % mod;
    return ans;
}

// Finds a primitive root for the given modulo dynamically.
// This is useful if you change the 'mod' variable and don't know its root.
int generator () {
    vector<int> fact;
    int phi = mod-1,  n = phi;
    
    // Prime factorize phi
    for (int i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0) n /= i;
        }
    if (n > 1) fact.push_back (n);

    // Test candidates to find the primitive root
    for (int res=2; res<=mod; ++res) {
        bool ok = true;
        for (size_t i=0; i<fact.size() && ok; ++i)
            ok &= modpow (res, phi / fact[i]) != 1;
        if (ok)  return res;
    }
    return -1;
}

// Computes (b^e) % m with a custom modulo 'm'
int modpow(int b, int e, int m) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// The core Number Theoretic Transform (NTT) algorithm
// Converts polynomial coefficients to point-values and vice versa
void ntt(vector<int> &a) {
    int n = (int)a.size(), L = 31 - __builtin_clz(n);
    vector<int> rt(2, 1); 
    
    // Precompute roots of unity
    for (int k = 2, s = 2; k < n; k *= 2, s++) { 
        rt.resize(n);
        int z[] = {1, modpow(root, mod >> s, mod)};
        for (int i = k; i < 2*k; ++i) rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
    }
    
    // Bit-reversal permutation
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
    
    // Iterative butterfly operations
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (ll)rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

// Multiplies two polynomials (a and b) modulo 'mod'
vector<int> conv(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    int s = (int)a.size() + (int)b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    vector<int> L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    
    ntt(L), ntt(R); // Convert to point-values
    for (int i = 0; i < n; ++i) out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod; // Multiply
    ntt(out); // Convert back to coefficients
    
    return {out.begin(), out.begin() + s};
}

// Chinese Remainder Theorem for 2 variables
// Solves: x = a (mod m1) and x = b (mod m2)
ll CRT(ll a, ll m1, ll b, ll m2) {
    __int128 m = m1*m2;
    ll ans = a*m2%m*modpow(m2, m1-2, m1)%m + m1*b%m*modpow(m1, m2-2, m2)%m;
    return ans % m;
}

/*
// --- ARBITRARY MODULO MULTIPLICATION SETUP ---
// This commented section uses 3 specific NTT-friendly primes.
// By computing the polynomial multiplication separately for mod1, mod2, and mod3,
// you can use the 3-variable CRT function below to find the exact answer modulo ANYTHING (like 10^9+7).

int mod, root, desired_mod = 1000000007;
const int mod1 = 167772161;
const int mod2 = 469762049;
const int mod3 = 754974721;
const int root1 = 3;
const int root2 = 3;
const int root3 = 11;

// Chinese Remainder Theorem for 3 variables
int CRT(int a, int b, int c, int m1, int m2, int m3) {
    __int128 M = (__int128)m1*m2*m3;
    ll M1 = (ll)m2*m3;
    ll M2 = (ll)m1*m3;
    ll M3 = (ll)m2*m1;

    int M_1 = modpow(M1%m1, m1 - 2, m1);
    int M_2 = modpow(M2%m2, m2 - 2, m2);
    int M_3 = modpow(M3%m3, m3 - 2, m3);

    __int128 ans = (__int128)a*M1*M_1;
    ans += (__int128)b*M2*M_2;
    ans += (__int128)c*M3*M_3;

    return (ans % M) % desired_mod;
}
*/