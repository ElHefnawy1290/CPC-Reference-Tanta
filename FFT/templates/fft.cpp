// Multiplies two polynomials (or large numbers) in O(N log N) time
// Uses the Fast Fourier Transform (FFT) to convert coefficients to point-values and back

#include <bits/stdc++.h>
using namespace std;
#define int long long

using cd = complex<double>;
const double PI = acos(-1);

// Fast Fourier Transform: 
// invert = false converts coefficients to point-values (Forward FFT)
// invert = true converts point-values back to coefficients (Inverse FFT)
void fft(vector<cd> &a, bool invert) {
    int n = a.size();

    // Bit-reversal permutation: Rearranges elements so we can build the FFT bottom-up iteratively
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    // Iterative FFT: Merge smaller subproblems into larger ones
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1.0, 0.0);
            for (int j = 0; j < len / 2; j++) {
                // Butterfly operation
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // If doing Inverse FFT, divide by N to complete the interpolation
    if (invert) {
        for (cd &x: a)
            x /= n;
    }
}

// Multiply two polynomials using FFT
vector<int> multiply(const vector<int> &a, const vector<int> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    
    // Pad the polynomial sizes to the nearest power of 2 (required for Radix-2 FFT)
    int n = 1;
    while (n < (int) (a.size() + b.size()))
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    // 1. Evaluate polynomials at complex roots of unity
    fft(fa, false);
    fft(fb, false);
    
    // 2. Point-wise multiplication in O(N) time
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
        
    // 3. Interpolate the results back into polynomial coefficients
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++)
        // Round to nearest integer to fix minor floating-point inaccuracies
        result[i] = round(fa[i].real());

    // Optional: trim trailing zeros
    while (!result.empty() && result.back() == 0)
        result.pop_back();

    return result;
}

// Optional: test usage
void solve() {
    int n;
    cin >> n;

    // Read coefficients from degree 0 up to degree n
    vector<int> a(n + 1), b(n + 1);
    for (int i = 0; i <= n; i++) cin >> a[i];
    for (int i = 0; i <= n; i++) cin >> b[i];

    // Get the multiplied polynomial
    vector<int> c = multiply(a, b);
    
    // The resulting polynomial has degree 2N, so we print 2N + 1 coefficients
    for (int i = 0; i <= 2*n; i++)
        // Handle cases where trailing zeros were trimmed
        cout << (i < c.size() ? c[i] : 0) << " ";
    cout << endl;
}

/*
If
a.size() = A (i.e. degree = A - 1)
b.size() = B (i.e. degree = B - 1)

Then:
✅ result.size() = A + B - 1 (before trimming)
 */

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while (t--) solve();
    return 0;
}