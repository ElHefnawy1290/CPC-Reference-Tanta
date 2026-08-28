/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: COMPRESSED BIGINT
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "No modulo given", "Massive addition/multiplication", "1000 digits".
 * - Classic Scenarios: DP state sums or combinatorics where the answer easily
 *   exceeds 2^64, but no modulo is provided.
 * - The Magic: C++ has no native BigInt. Using strings character-by-character
 *   is O(N^2) with a massive constant factor. This struct bundles 9 decimal digits
 *   into a single integer, processing data 9x faster and using significantly less memory.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       BigInt a = "12345678901234567890";
 *       BigInt b = 987654321;
 * - Operations:
 *       BigInt c = a + b;
 *       c.print();
 */

const int BASE = 1e9;

struct BigInt
{
    vector<int> a;

    BigInt() {}
    BigInt(long long v)
    {
        while (v > 0)
        {
            a.push_back(v % BASE);
            v /= BASE;
        }
    }
    BigInt(string s)
    {
        if (s.empty())
            return;
        for (int i = s.length(); i > 0; i -= 9)
        {
            if (i < 9)
                a.push_back(stoll(s.substr(0, i)));
            else
                a.push_back(stoll(s.substr(i - 9, 9)));
        }
        trim();
    }

    void trim()
    {
        while (a.size() > 1 && a.back() == 0)
            a.pop_back();
    }

    BigInt operator+(const BigInt &b) const
    {
        BigInt res;
        int carry = 0;
        for (int i = 0; i < max(a.size(), b.a.size()) || carry; i++)
        {
            long long cur = carry;
            if (i < a.size())
                cur += a[i];
            if (i < b.a.size())
                cur += b.a[i];
            res.a.push_back(cur % BASE);
            carry = cur / BASE;
        }
        return res;
    }

    BigInt operator*(const BigInt &b) const
    {
        BigInt res;
        if (a.empty() || b.a.empty())
            return res;
        res.a.assign(a.size() + b.a.size(), 0);
        for (int i = 0; i < a.size(); i++)
        {
            long long carry = 0;
            for (int j = 0; j < b.a.size() || carry > 0; j++)
            {
                long long cur = res.a[i + j] + a[i] * 1LL * (j < b.a.size() ? b.a[j] : 0) + carry;
                res.a[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.trim();
        return res;
    }

    bool operator<(const BigInt &b) const
    {
        if (a.size() != b.a.size())
            return a.size() < b.a.size();
        for (int i = a.size() - 1; i >= 0; i--)
        {
            if (a[i] != b.a[i])
                return a[i] < b.a[i];
        }
        return false;
    }

    void print() const
    {
        if (a.empty())
        {
            cout << 0 << "\n";
            return;
        }
        cout << a.back();
        for (int i = (int)a.size() - 2; i >= 0; i--)
        {
            cout << setfill('0') << setw(9) << a[i];
        }
        cout << "\n";
    }
};