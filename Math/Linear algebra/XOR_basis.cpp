/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: XOR BASIS (LINEAR BASIS)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Maximum XOR subset", "K-th XOR sum", "Can we form X".
 * - Classic Scenarios: You are given an array of size 10^5 and need to find the
 *   maximum possible XOR sum of any subset.
 * - The Magic: It reduces the entire array into a set of at most B "basis" vectors
 *   (where B is the number of bits, usually 60 for long long). These basis vectors
 *   span the exact same XOR space as the original array.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       XORBasis basis;
 *       for (ll x : arr) basis.insert(x);
 * - Queries:
 *       long long max_val = basis.max_xor();
 *       bool exists = basis.contains(X);
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Reduced Row Echelon Form (RREF) / K-th Smallest: To answer "K-th smallest XOR sum"
 *   (Topic 383/384), you MUST call `basis.build_rref()` first. Then use `basis.kth(K)`.
 *   (Note: K is 1-indexed. If K=1, it returns the smallest possible XOR sum > 0).
 */

using ll = long long;

struct XORBasis
{
    static const int BITS = 60; // Max bits for long long
    vector<ll> basis;
    vector<ll> rref; // For K-th queries
    int sz;
    bool is_rref_built;

    XORBasis()
    {
        basis.assign(BITS, 0);
        sz = 0;
        is_rref_built = false;
    }

    bool insert(ll x)
    {
        for (int i = BITS - 1; i >= 0; i--)
        {
            if ((x >> i) & 1)
            {
                if (!basis[i])
                {
                    basis[i] = x;
                    sz++;
                    is_rref_built = false;
                    return true;
                }
                x ^= basis[i];
            }
        }
        return false;
    }

    bool contains(ll x)
    {
        for (int i = BITS - 1; i >= 0; i--)
        {
            if ((x >> i) & 1)
            {
                if (!basis[i])
                    return false;
                x ^= basis[i];
            }
        }
        return true;
    }

    ll max_xor(ll current_xor = 0)
    {
        ll res = current_xor;
        for (int i = BITS - 1; i >= 0; i--)
        {
            res = max(res, res ^ basis[i]);
        }
        return res;
    }

    // Call this strictly BEFORE querying kth()
    void build_rref()
    {
        if (is_rref_built)
            return;
        rref.clear();
        vector<ll> temp = basis;
        for (int i = BITS - 1; i >= 0; i--)
        {
            if (!temp[i])
                continue;
            for (int j = i + 1; j < BITS; j++)
            {
                if ((temp[j] >> i) & 1)
                    temp[j] ^= temp[i];
            }
        }
        for (int i = 0; i < BITS; i++)
        {
            if (temp[i])
                rref.push_back(temp[i]);
        }
        is_rref_built = true;
    }

    // Returns K-th smallest XOR sum (1-indexed, excluding 0)
    // Returns -1 if K is out of bounds
    ll kth(ll k)
    {
        build_rref();
        if (k > (1LL << sz) - 1)
            return -1;
        ll res = 0;
        for (int i = 0; i < rref.size(); i++)
        {
            if ((k >> i) & 1)
                res ^= rref[i];
        }
        return res;
    }
};