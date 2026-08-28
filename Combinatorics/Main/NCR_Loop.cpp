// check not overflow
int ncr(int n, int r) {
    if (r > n || n < 0 || r < 0) return 0;
    if (r == 0 || r == n) return 1;
    if (r > n - r) {
        r = n - r;
    }
    int res = 1;
    for (int i = 1; i <= r; ++i) {
        res *= (n - r + i);
        res /= i;
    }
    return res;

}
