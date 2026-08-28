// Arbitrary Modulo Polynomial Convolution (MTT) in O(N log N) time
// Multiplies two polynomials modulo ANY number (does not require NTT primes)

#include <bits/stdc++.h>
using namespace std;

#define rep(aa, bb, cc) for(int aa = bb; aa < cc;aa++)
#define sz(a) (int)a.size()
typedef long long ll;
typedef vector<int> vi;
typedef complex<double> C;
typedef vector<double> vd;

// Highly optimized Fast Fourier Transform
void fft(vector<C>& a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);  // Caches roots of unity for speed
    
    // Precompute roots of unity
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n); rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        rep(i,k,2*k) rt[i] = R[i] = i&1 ? R[i/2] * x : R[i/2];
    }
    
    // Bit-reversal permutation
    vi rev(n);
    rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
    
    // Iterative FFT
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
                // Hand-rolled complex multiplication for a ~25% speed boost
                auto x = (double *)&rt[j+k], y = (double *)&a[i+j+k];        
                C z(x[0]*y[0] - x[1]*y[1], x[0]*y[1] + x[1]*y[0]);           
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}
 
// Arbitrary Modulo Convolution
template<int M> vi convMod(const vi &a, const vi &b) {
    if (a.empty() || b.empty()) return {};
    vi res(sz(a) + sz(b) - 1);
    
    // Pad to nearest power of 2
    int B=32-__builtin_clz(sz(res)), n=1<<B;
    
    // Split values into chunks of roughly sqrt(M) to prevent floating-point overflow
    int cut=int(sqrt(M)); 
    vector<C> L(n), R(n), outs(n), outl(n);
    
    // Encode polynomial A as (A_div + i * A_mod) and B as (B_div + i * B_mod)
    rep(i,0,sz(a)) L[i] = C((int)a[i] / cut, (int)a[i] % cut);
    rep(i,0,sz(b)) R[i] = C((int)b[i] / cut, (int)b[i] % cut);
    fft(L), fft(R);
    
    // Clever math trick to extract the transforms of the real/imaginary parts using conjugates
    rep(i,0,n) {
        int j = -i & (n - 1); // Equivalent to (n - i) % n
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
    }
    
    // Inverse FFT to get the cross-multiplications
    fft(outl), fft(outs);
    
    // Recombine the pieces modulo M
    rep(i,0,sz(res)) {
        ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
        ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}

// 
const int MOD = 1e9 + 7;
vector<int> a = {1, 2, 3};
vector<int> b = {4, 5, 6};

vector<int> result = convMod<MOD>(a, b);