struct NumberTheory {
    int n;
    vector<int> p, spf, phi_arr, fac, inv, finv;
    vector<bool> isPrime;
    vector<vector<int> > divisor;

    NumberTheory(int max_n = 1000000, bool pre_divs = false) {
        n = max_n;
        isPrime.assign(n + 1, true);
        spf.assign(n + 1, 0);
        phi_arr.assign(n + 1, 0);
        isPrime[0] = isPrime[1] = false;

        for (int i = 0; i <= n; i++) phi_arr[i] = i;

        for (int i = 2; i <= n; i++) {
            if (isPrime[i]) {
                p.push_back(i);
                spf[i] = i;
                phi_arr[i] -= phi_arr[i] / i;
            }
            for (int j = 0; j < (int) p.size() && i * p[j] <= n; j++) {
                isPrime[i * p[j]] = false;
                spf[i * p[j]] = p[j];
                if (i % p[j] == 0) {
                    phi_arr[i * p[j]] = phi_arr[i] * p[j];
                    break;
                } else {
                    phi_arr[i * p[j]] = phi_arr[i] * phi_arr[p[j]];
                }
            }
        }

        fac.assign(n + 1, 1);
        inv.assign(n + 1, 1);
        finv.assign(n + 1, 1);
        for (int i = 1; i <= n; i++) fac[i] = mul(fac[i - 1], i);
        for (int i = 2; i <= n; i++) inv[i] = (MOD - (MOD / i) * inv[MOD % i] % MOD) % MOD;
        for (int i = 2; i <= n; i++) finv[i] = mul(finv[i - 1], inv[i]);

        if (pre_divs) {
            divisor.resize(n + 1);
            for (int i = 1; i <= n; i++) {
                for (int j = i; j <= n; j += i) {
                    divisor[j].push_back(i);
                }
            }
        }
    }

    int add(int a, int b) { return ((a % MOD) + (b % MOD)) % MOD; }
    int sub(int a, int b) { return (((a - b) % MOD) + MOD) % MOD; }
    int mul(int a, int b) { return ((a % MOD) * (b % MOD)) % MOD; }

    int power(int b, int pow) {
        b %= MOD;
        int res = 1;
        while (pow) {
            if (pow & 1) res = mul(res, b);
            b = mul(b, b);
            pow >>= 1;
        }
        return res;
    }

    int modInv(int x) { return power(x, MOD - 2); }
    int divide(int a, int b) { return mul(a, modInv(b)); }

    int nCr(int x, int y) {
        if (x < 0 || y > x || y < 0) return 0;
        return mul(fac[x], mul(finv[y], finv[x - y]));
    }

    int nPr(int x, int y) {
        if (x < 0 || y > x || y < 0) return 0;
        return mul(fac[x], finv[x - y]);
    }

    int catalan(int x) { return mul(nCr(2 * x, x), modInv(x + 1)); }
    int StarsAndPars(int x, int k) { return nCr(x + k - 1, k - 1); }

    int nCr_no_mod(int x, int r) {
        if (r < 0 || x < 0 || r > x) return 0;
        int ret = 1;
        for (int i = 1; i <= min(r, x - r); i++) {
            ret *= x - i + 1;
            ret /= i;
        }
        return ret;
    }

    int nPr_no_mod(int x, int r) {
        if (r < 0 || x < 0 || r > x) return 0;
        int ret = 1;
        for (int i = 0; i < r; i++) ret *= (x - i);
        return ret;
    }

    int numberOfDivisors1ToN(int limit) {
        int sum = 0;
        for (int i = 1; i <= limit; i++) sum += limit / i;
        return sum;
    }

    bool is_prime(int x) {
        if (x <= 1) return false;
        if (x % 2 == 0) return x == 2;
        for (int i = 3; i * i <= x; i += 2) {
            if (x % i == 0) return false;
        }
        return true;
    }

    vector<int> factorization(int x) {
        vector<int> primes_factors;
        for (int i = 2; i * i <= x; i++) {
            while (x % i == 0) {
                primes_factors.push_back(i);
                x /= i;
            }
        }
        if (x > 1) primes_factors.push_back(x);
        return primes_factors;
    }

    vector<int> spf_factorization(int x) {
        vector<int> primes_factors;
        while (x != 1) {
            primes_factors.push_back(spf[x]);
            x /= spf[x];
        }
        return primes_factors;
    }

    map<int, int> spf_factorization_freq(int x) {
        map<int, int> frq;
        while (x != 1) {
            frq[spf[x]]++;
            x /= spf[x];
        }
        return frq;
    }

    int phi(int x) {
        int ret = x;
        for (int i = 2; i * i <= x; i++) {
            if (x % i == 0) {
                while (x % i == 0) x /= i;
                ret -= ret / i;
            }
        }
        if (x > 1) ret -= ret / x;
        return ret;
    }
};
    