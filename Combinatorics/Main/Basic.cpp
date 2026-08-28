// ============================================================================
// MODULE 1: MODULAR ARITHMETIC & CORE COMBINATORICS
// ============================================================================
struct Combinatorics
{
    int N;
    vector<ll> fact, invFact;

    // Fast Modular Exponentiation: Computes (base^exp) % MOD in O(log exp)
    ll power(ll base, ll exp)
    {
        ll res = 1;
        base %= MOD;
        while (exp > 0)
        {
            if (exp % 2 == 1)
                res = (res * base) % MOD;
            base = (base * base) % MOD;
            exp /= 2;
        }
        return res;
    }

    // Modular Multiplicative Inverse: Computes (1 / n) % MOD in O(log MOD)
    // Uses Fermat's Little Theorem (Requires MOD to be prime)
    ll modInverse(ll n)
    {
        return power(n, MOD - 2);
    }

    // Initialize Factorials and Inverse Factorials in O(N)
    // CALL THIS ONCE IN MAIN BEFORE SOLVING TEST CASES!
    void init(int n)
    {
        N = n;
        fact.resize(N + 1);
        invFact.resize(N + 1);

        fact[0] = 1;
        invFact[0] = 1;
        for (int i = 1; i <= N; i++)
        {
            fact[i] = (fact[i - 1] * i) % MOD;
        }

        // Compute the inverse of the largest factorial, then work backwards
        invFact[N] = modInverse(fact[N]);
        for (int i = N - 1; i >= 1; i--)
        {
            invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
        }
    }

    // Choose: Number of ways to pick r items from n items. O(1)
    ll nCr(int n, int r)
    {
        if (r < 0 || r > n)
            return 0;
        return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
    }

    // Permutation: Number of ways to arrange r items out of n items. O(1)
    ll nPr(int n, int r)
    {
        if (r < 0 || r > n)
            return 0;
        return fact[n] * invFact[n - r] % MOD;
    }

    // Stars and Bars: Ways to distribute n identical items into k distinct bins. O(1)
    // Example: Solutions to x1 + x2 + ... + xk = n (where xi >= 0)
    ll starsAndBars(int n, int k)
    {
        if (n == 0 && k == 0)
            return 1;
        return nCr(n + k - 1, k - 1);
    }

    // Stars and Bars (Positive): Distribute n identical items into k distinct bins,
    // such that every bin gets AT LEAST ONE item. O(1)
    // Example: Solutions to x1 + x2 + ... + xk = n (where xi >= 1)
    ll starsAndBarsPositive(int n, int k)
    {
        if (n < k)
            return 0;
        return nCr(n - 1, k - 1);
    }

    // Catalan Numbers: O(1)
    // Solves: Valid parenthesis expressions, binary trees, polygon triangulations.
    // Formula: C(n) = (1 / (n + 1)) * (2n C n)
    ll catalan(int n)
    {
        if (n < 0)
            return 0;
        ll res = nCr(2 * n, n);
        res = (res * modInverse(n + 1)) % MOD;
        return res;
    }
};

// ============================================================================
// MODULE 2: ADVANCED COUNTING (DERANGEMENTS, STIRLING, BELL)
// ============================================================================
struct AdvancedCombinatorics
{
    // Derangements: Number of permutations of N elements where NO element is in its original position.
    // O(N) precomputation. Formula: D(n) = (n-1) * (D(n-1) + D(n-2))
    vector<ll> derangement;

    void initDerangements(int n)
    {
        derangement.assign(n + 1, 0);
        if (n >= 0)
            derangement[0] = 1;
        if (n >= 1)
            derangement[1] = 0;
        for (int i = 2; i <= n; i++)
        {
            derangement[i] = ((i - 1) * (derangement[i - 1] + derangement[i - 2]) % MOD) % MOD;
        }
    }

    // Stirling Numbers of the First Kind: S1[n][k]
    // Solves: Number of permutations of 'n' elements that contain exactly 'k' disjoint cycles.
    // O(N^2) precomputation.
    vector<vector<ll>> S1;

    void initStirling1(int n)
    {
        S1.assign(n + 1, vector<ll>(n + 1, 0));
        S1[0][0] = 1;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                S1[i][j] = (S1[i - 1][j - 1] + (i - 1) * S1[i - 1][j]) % MOD;
            }
        }
    }

    // Stirling Numbers of the Second Kind: S2[n][k]
    // Solves: Number of ways to partition a set of 'n' DISTINCT elements into exactly 'k' non-empty indistinguishable subsets.
    // O(N^2) precomputation.
    vector<vector<ll>> S2;

    void initStirling2(int n)
    {
        S2.assign(n + 1, vector<ll>(n + 1, 0));
        S2[0][0] = 1;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= i; j++)
            {
                S2[i][j] = (S2[i - 1][j - 1] + j * S2[i - 1][j]) % MOD;
            }
        }
    }

    // Bell Numbers: Bell[n]
    // Solves: Total number of ways to partition a set of 'n' elements into ANY number of non-empty subsets.
    // O(N^2) precomputation (Sum of Stirling2(n, k) for k = 0 to n).
    vector<ll> bell;

    void initBell(int n)
    {
        if (S2.empty())
            initStirling2(n);
        bell.assign(n + 1, 0);
        for (int i = 0; i <= n; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                bell[i] = (bell[i] + S2[i][j]) % MOD;
            }
        }
    }
};

// ============================================================================
// MODULE 3: LUCAS THEOREM (LARGE N, SMALL PRIME MODULO)
// ============================================================================
// Solves: nCr % P when N and R are massive (up to 10^18) but P is small (up to 10^5) and P is PRIME.
// Note: This requires a separate Combinatorics instance initialized up to P.
ll lucas_nCr(ll n, ll r, int p, Combinatorics &combo)
{
    if (r < 0 || r > n)
        return 0;
    if (r == 0)
        return 1;

    // Split n and r into base P representation
    ll ni = n % p;
    ll ri = r % p;

    if (ni < ri)
        return 0; // Optimization: P divides nCr

    // nCr = Product of (n_i C r_i) mod P
    return (combo.nCr(ni, ri) * lucas_nCr(n / p, r / p, p, combo)) % p;
}
