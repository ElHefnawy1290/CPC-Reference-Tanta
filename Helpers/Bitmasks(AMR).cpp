//
// Created by AMR on 4/16/2025.
//

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define El_Hefnawys ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr)

const int MOD = 1e9 + 7;



// A^B = (A|B) - (A&B)
// A^B = (A+B) - 2*(A&B)
// A^B = 2*(A|B) - (A+B)

bool is_power_2(int x) {
    if (x == 0)
        return 0;
    return x&(x-1) == 0;
}

int MSB_value(int n) {
    if (n == 0)
        return 0;
    int index = 31 - __builtin_clz(n);
    return 1 << index;
}

int MSB_index(int n) {
    if (n == 0)
        return -1;
    return 31 - __builtin_clz(n);
}

// you can use __builtin_ctz(n) to get the LSB also directly
int LSB_value(int n) {
    if (n == 0)
        return 0;
    return n&(-n);
}

int LSB_index(int n) {
    if (n == 0)
        return -1;
    int value = n&(-n);
    return 31 - __builtin_clz(value);
}

bool check_bit(ll n, ll i) {
    return n&(1ll<<i);
}

// Another way
bool check_bit_2(ll n, ll i) {
    return 1ll&(n>>i);
}

ll toggle_bit(ll n, ll i) {
    return n^(1ll<<i);
}

ll set_bit(ll n, ll i) {
    return n | (1ll<<i);
}

ll clear_bit(ll n, ll i) {
    return n & (~(1ll<<i));
}

bool check_pow_2(ll n) {
    return __builtin_popcountll(n) == 1; // function to count the number of 1s in a number
    // O(1) function
}

ll num_modulo_pow_2(ll n, ll k) {
    return n & ((1ll << k) - 1);
}

int main() {
    El_Hefnawys;
    cout << num_modulo_pow_2(25,4);
    return 0;
}

//////////////////////////

//
// Created by AMR on 4/17/2025.
//

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define El_Hefnawys                   \
    ios_base::sync_with_stdio(false); \
    cin.tie(nullptr);                 \
    cout.tie(nullptr)

const int MOD = 1e9 + 7;

int Xor_nums_from_1_to_n(int n)
{
    if (!(n % 4))
        return n;
    if (n % 4 == 1)
        return 1;
    if (n % 4 == 2)
        return n + 1;
    return 0;
}