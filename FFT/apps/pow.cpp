vector<int> poly_pow(vector<int> poly, int p) {
    vector<int> ans{1};
    while (p) {
        if(p&1) ans = multiply(ans, poly);
        poly = multiply(poly, poly);
        p >>= 1;
    }
    return ans;
}