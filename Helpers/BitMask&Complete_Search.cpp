
vector <ll> getRepresention(ll num, ll base) // convert any number to (any)base 
{
    vector<ll>ans;
    while (num)
    {
        ans.push_back(num % base);
        num /= base;
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
 
// 1. Check if the i-th bit is set (1) or not (0)
bool checkBit(long long mask, int i) {
    return (mask & (1LL << i)) != 0;
}

// 2. Set the i-th bit to 1 (leave other bits unchanged)
long long setBit(long long mask, int i) {
    return mask | (1LL << i);
}

// 3. Clear the i-th bit to 0 (leave other bits unchanged)
long long clearBit(long long mask, int i) {
    return mask & ~(1LL << i);
}

// 4. Toggle the i-th bit (flip 1 to 0, or 0 to 1)
long long toggleBit(long long mask, int i) {
    return mask ^ (1LL << i);
}

// 5. Get the value of the lowest set bit (e.g., 1010 -> 0010)
// Useful for Fenwick trees (Binary Indexed Trees)
long long lowestSetBit(long long mask) {
    return mask & -mask; 
}

// 6. Check if a number is a perfect power of 2
// A power of 2 has exactly one bit set (e.g., 8 is 1000 in binary)
bool isPowerOfTwo(long long n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// 7. Strip the lowest set bit (change the rightmost 1 to a 0)
long long stripLowestSetBit(long long mask) {
    return mask & (mask - 1);
}
int LastBitValue(ll n )
{
    return n & (~(n - 1));
}

// 1. Count the number of set bits (1s) in a 32-bit integer
int count = __builtin_popcount(mask);

// 2. Count the number of set bits (1s) in a 64-bit integer (long long)
int countLL = __builtin_popcountll(mask);

// 3. Count the number of leading zeros
int leadingZeros = __builtin_clz(mask);

// 4. Count the number of trailing zeros (useful to find the index of the first set bit)
int trailingZeros = __builtin_ctz(mask);

 ////////// Ranges ////////////////

void prefix_Xor()
{
    int n; cin >> n; vector<ll>v(n + 1);
    for (int i = 1; i <= n; i++) cin >> v[i];
    vector<ll>pre_Xor(n + 1);
    for (int i = 1; i <= n; i++)
        pre_Xor[i] = pre_Xor[i - 1] ^ v[i];   
    int q; cin >> q; while (q--) {
        int l, r; cin >> l >> r;
        cout << (pre_Xor[r] ^ pre_Xor[l - 1]) << '\n';
    }
}
void prefix_OR()
{
    int n; cin >> n; vector<ll>v(n + 1);
    vector<vector<ll>>prefix_bit(64, vector<ll>(n + 1));
    for (int i = 1; i <= n; i++) cin >> v[i];

    for (int bit = 0; bit < 64; bit++)
    {
        for (int i = 1; i <= n; i++)
        {
            prefix_bit[bit][i] = prefix_bit[bit][i - 1] + ((v[i] >> bit) & 1);
        }
    }
    int q; cin >> q; while (q--) {
        int l, r; cin >> l >> r;
        ll OR = 0;
        for (int bit = 0; bit < 64; bit++)
        {
            int on_bits = prefix_bit[bit][r] - prefix_bit[bit][l - 1]; 
            if (on_bits > 0)
            {
                OR += (1LL << bit);
            }
        }
        cout << OR << '\n';
    }
}
void prefix_AND()
{
    int n; cin >> n; vector<ll>v(n + 1);
    vector<vector<ll>>prefix_bit(64, vector<ll>(n + 1));
    for (int i = 1; i <= n; i++) cin >> v[i];

    for (int bit = 0; bit < 64; bit++)
    {
        for (int i = 1; i <= n; i++)
        {
            prefix_bit[bit][i] = prefix_bit[bit][i - 1] + ((v[i] >> bit) & 1);
        }
    }
    int q; cin >> q; while (q--) {
        int l, r; cin >> l >> r;
        ll AND = 0;
        for (int bit = 0; bit < 64; bit++)
        {
            int on_bits = prefix_bit[bit][r] - prefix_bit[bit][l - 1];
            if (on_bits == r-l+1)
            {
                AND += (1LL << bit);
            }
        }
        cout << AND << '\n';
    }
}


void CompleteSearch()
{
    int n; cin >> n;
    for (int mask = 0; mask < (1 << n); mask++)
    {
        for (int i = 0; i < n; i++)
        {
            if ((mask >> i) &1)
            //if (getBit(mask,i))
                cout << 1;
            else
                cout << 0;
        }
        cout << '\n';
    }
}

void iterateSubsets(int mask) {
    // Start with the full mask, and keep stripping away bits
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // 'sub' is a valid subset of 'mask'
        std::cout << sub << "\n";
    }
    // Note: The loop above misses the empty subset (0). 
    // If you need to process 0 as well, you can do it after the loop.
}


void BitSet() {
    bitset<32> b = 15;

    b[3] = 0;
    cout << b.to_ullong() << '\n'; // 7
    cout << b << '\n';  // 00000000000000000000000000000111
    cout << b.size() << "\n"; //32 bit
    cout << b.test(2) << '\n'; // 1 >> true
    cout << b.test(3) << '\n'; // 0 >> false
    cout << b.count() << '\n'; // Count number of 1  == 3
    cout << b.flip() << '\n';  // 11111111111111111111111111111000
    cout << b.set() << '\n';   //11111111111111111111111111111111
    cout << b.reset() << '\n'; //00000000000000000000000000000000
    b[4] = 1;
    cout << b.to_string() << '\n'; //00000000000000000000000000010000
}
