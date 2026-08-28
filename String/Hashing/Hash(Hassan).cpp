const int mod1 = 1e9 + 7;
const int mod2 = 1e9 + 9;
const int base1 = 313;
const int base2 = 317;

const int MAXN = 3e4 + 100;
int pw1[MAXN], pw2[MAXN];
bool isCalPower = false;

void pre() {
    if (isCalPower) return;

    pw1[0] = pw2[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        pw1[i] = (pw1[i - 1] * base1) % mod1;
        pw2[i] = (pw2[i - 1] * base2) % mod2;
    }
    isCalPower = true;
}

struct Hash {
    int n;
    vector<pair<int, int>> pref;
    vector<pair<int, int>> suf; // Used only if palindrome checking is needed
 
    // Pass `compute_rev = true` if you need O(1) palindrome checks
    Hash(const string& s, bool compute_rev = false) {
        pre();
        n = s.size();
        pref.assign(n + 1, {0, 0});

        for (int i = 0; i < n; i++) {
            pref[i + 1].first = (pref[i].first * base1 + s[i]) % mod1;
            pref[i + 1].second = (pref[i].second * base2 + s[i]) % mod2;
        }

        if (compute_rev) {
            suf.assign(n + 1, {0, 0});
            for (int i = n - 1; i >= 0; i--) {
                suf[i].first = (suf[i + 1].first * base1 + s[i]) % mod1;
                suf[i].second = (suf[i + 1].second * base2 + s[i]) % mod2;
            }
        }
    }

    // Get hash of substring s[l...r] (0-indexed) in O(1)
    pair<int, int> get(int l, int r) {
        if (l > r) return {0, 0};
        int len = r - l + 1;
        
        int h1 = (pref[r+1].first - (pref[l].first * pw1[len]) % mod1) % mod1;
        if (h1 < 0) h1 += mod1;

        int h2 = (pref[r+1].second - (pref[l].second * pw2[len]) % mod2) % mod2;
        if (h2 < 0) h2 += mod2;

        return {h1, h2};
    }

    // Get reverse hash of substring s[l...r] in O(1)
    pair<int, int> get_rev(int l, int r) {
        if (l > r || suf.empty()) return {0, 0};
        int len = r - l + 1;
        int h1 = (suf[l].first - suf[r + 1].first * pw1[len]) % mod1;
        if (h1 < 0) h1 += mod1;

        int h2 = (suf[l].second - suf[r + 1].second * pw2[len]) % mod2;
        if (h2 < 0) h2 += mod2;

        return {h1, h2};
    }

    // Check if s[l...r] is a palindrome in O(1)
    bool is_pal(int l, int r) {
        return get(l, r) == get_rev(l, r);
    }
};


// --- USEFUL HASHING UTILITY FUNCTIONS ---

// Concatenate two hashes (h1 followed by h2 of length len2) in O(1)
pair<int, int> concat_hash(pair<int, int> h1, pair<int, int> h2, int len2) {
    int ans1 = (h1.first * pw1[len2] + h2.first) % mod1;
    int ans2 = (h1.second * pw2[len2] + h2.second) % mod2;
    return {ans1, ans2};
}

// Find Longest Common Prefix (LCP) length of s1[l1...r1] and s2[l2...r2] in O(log N)
int get_lcp(Hash& sh1, int l1, int r1, Hash& sh2, int l2, int r2) {
    int low = 1, high = min(r1 - l1 + 1, r2 - l2 + 1), ans = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sh1.get(l1, l1 + mid - 1) == sh2.get(l2, l2 + mid - 1)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

// Compare s1[l1...r1] and s2[l2...r2] lexicographically in O(log N)
// Returns: -1 if s1 < s2, 0 if s1 == s2, 1 if s1 > s2
int compare_substrings(const string& s1, Hash& sh1, int l1, int r1, 
                       const string& s2, Hash& sh2, int l2, int r2) {
    int lcp_len = get_lcp(sh1, l1, r1, sh2, l2, r2);
    int len1 = r1 - l1 + 1;
    int len2 = r2 - l2 + 1;
    
    // If one is completely a prefix of another
    if (lcp_len == len1 && lcp_len == len2) return 0;
    if (lcp_len == len1) return -1;
    if (lcp_len == len2) return 1;
    
    // Compare the first differing character
    return s1[l1 + lcp_len] < s2[l2 + lcp_len] ? -1 : 1;
}
