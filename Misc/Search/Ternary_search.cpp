#include <bits/stdc++.h>
using namespace std;

// The function we want to minimize (e.g., total cost, distance, time)
// For this example, let's pretend it's a parabola: f(x) = (x - 5)^2 + 10
// The minimum should clearly be at x = 5.
double f_double(double x)
{
    return (x - 5.0) * (x - 5.0) + 10.0;
}

long long f_int(long long x)
{
    return (x - 5) * (x - 5) + 10;
}

// ---------------------------------------------------------
// TEMPLATE 1: Continuous Ternary Search (Floating Point)
// ---------------------------------------------------------
double ternary_search_double(double l, double r)
{
    // THE CP TRICK: Never use `while (r - l > eps)`. Floating point precision
    // errors can cause an infinite loop and TLE your submission.
    // Instead, looping exactly 200 times guarantees immense precision
    // (the range shrinks by 2/3^200, which is basically 0).
    for (int i = 0; i < 200; i++)
    {
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;

        if (f_double(m1) < f_double(m2))
        {
            r = m2; // We are minimizing, so throw away the higher right side
        }
        else
        {
            l = m1; // Throw away the higher left side
        }
    }
    return l; // l and r are essentially identical now
}

// ---------------------------------------------------------
// TEMPLATE 2: Discrete Ternary Search (Integers)
// ---------------------------------------------------------
long long ternary_search_int(long long l, long long r)
{
    // We stop when the search space shrinks to 3 elements or less.
    // If we try to split a range of size 2 into thirds with integers,
    // m1 and m2 will evaluate to the same number and cause an infinite loop!
    while (r - l > 2)
    {
        long long m1 = l + (r - l) / 3;
        long long m2 = r - (r - l) / 3;

        if (f_int(m1) < f_int(m2))
        {
            r = m2;
        }
        else
        {
            l = m1;
        }
    }

    // Now we have a tiny range (l, l+1, r). Just brute force the answer!
    long long best_x = l;
    long long min_val = f_int(l);

    for (long long i = l + 1; i <= r; i++)
    {
        if (f_int(i) < min_val)
        {
            min_val = f_int(i);
            best_x = i;
        }
    }

    return best_x;
}

void solve()
{
    cout << "Continuous Minimum at X = " << ternary_search_double(-1e9, 1e9) << "\n";
    cout << "Discrete Minimum at X = " << ternary_search_int(-1000000, 1000000) << "\n";
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
