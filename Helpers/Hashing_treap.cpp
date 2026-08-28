/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: IMPLICIT TREAP WITH STRING HASHING
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Dynamic String", "Insert/Delete characters", "Shift indices", "Palindrome queries".
 * - Classic Scenarios: A Segment Tree can update a character at index X, but it CANNOT insert 
 *   a new character (shifting everything right) or delete a substring (shifting everything left). 
 *   If a problem asks you to insert/delete characters AND query substrings/palindromes, 
 *   standard Segment Trees or Fenwick trees will fail.
 * - The Magic: "The Implicit Treap". It acts as a dynamic array where every node represents 
 *   a character. By using the size of the left subtree as the "implicit index", we can Split 
 *   and Merge the string in O(log N) time. We augment each node to store the Rolling Hash of 
 *   its subtree. When we merge nodes, we mathematically combine their hashes using precomputed 
 *   powers of our base.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: You MUST call `init()` in `main()` to precompute the `pw` array.
 *       Treap a(s), b(reversed_s);
 * - Insertions: Insert character `c` at 1-based position `pos`.
 *       a.addChar(pos, c);
 * - Deletions: Delete the entire substring from 1-based index `L` to `R`.
 *       a.deleteRange(L, R);
 * - Queries: Get the rolling hash of the substring [L, R].
 *       int h = a.getHash(L, R);
 *
 * - Complexity:
 *       Time: O(log N) for Insert, Delete, and Query!
 *       Space: O(N) for the Treap nodes.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - The Palindrome Reverse Math: When tracking a reversed string dynamically, indices flip. 
 *   If the current string length is `N`, the equivalent of index `idx` in the reversed string 
 *   is `N - idx + 1`. Note that `N` MUST be manually updated (e.g., `n++` or `n -= length`) 
 *   exactly as shown in `main()` whenever you insert or delete!
 * - Memory Limit Exceeded (MLE) Warning: In `deleteRange`, the code drops the `b[0]` component 
 *   (the deleted segment) without freeing the memory. In competitive programming, this is 
 *   usually fine. But if you face MLE on huge test cases, you should write a recursive `destroy()` 
 *   function to `delete` the extracted nodes.
 * - Single vs Double Hashing: This template uses Single Hashing (`mod = 1e9+7`). If you get 
 *   Wrong Answer (WA) due to malicious anti-hash tests, you can upgrade `h` to a `pair<int,int>` 
 *   and apply double hashing just like in the Segment Tree template.
 */
#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define int long long
const int N = 1e6 + 5, mod = 1e9 + 7;
ll base = 31, pw[N + 1];

void init()
{
    pw[0] = 1;
    for (int i = 1; i < N; i++)
    {
        pw[i] = (base * pw[i - 1]) % mod;
    }
}

std::mt19937_64 rnd(std::chrono::system_clock::now().time_since_epoch().count());

struct Treap
{
    struct Node
    {
        int val, h = 0;
        int pri = rnd(), sz = 1;
        array<Node *, 2> c = {0, 0};

        Node()
        {
        }

        Node(int k)
        {
            val = h = k;
        }
    };

    Node *root = 0;
    int getSize(Node *t)
    {
        return t ? t->sz : 0;
    }

    int getHash(Node *t)
    {
        return t ? t->h : 0;
    }

    Treap(string &s)
    {
        for (int i = 0; i < s.size(); i++)
        {
            root = merge(root, new Node(s[i] - 'a' + 1));
        }
    }

    Node *fix(Node *t)
    {
        t->sz = getSize(t->c[0]) + getSize(t->c[1]) + 1;
        t->h = (((getHash(t->c[0]) * base + t->val) % mod) * pw[getSize(t->c[1])] + getHash(t->c[1])) % mod;
        return t;
    }

    array<Node *, 2> split(Node *t, int k)
    {
        if (!t)
            return {0, 0};
        if (getSize(t->c[0]) >= k)
        {
            auto ret = split(t->c[0], k);
            t->c[0] = ret[1];
            return {ret[0], fix(t)};
        }
        else
        {
            auto ret = split(t->c[1], k - getSize(t->c[0]) - 1);
            t->c[1] = ret[0];
            return {fix(t), ret[1]};
        }
    }

    Node *merge(Node *u, Node *v)
    {
        if (!u || !v)
            return u ? u : v;
        if (u->pri > v->pri)
        {
            u->c[1] = merge(u->c[1], v);
            return fix(u);
        }
        else
        {
            v->c[0] = merge(u, v->c[0]);
            return fix(v);
        }
    }

    void deleteRange(int l, int r)
    {
        auto a = split(root, l - 1);
        auto b = split(a[1], r - l + 1);
        root = merge(a[0], b[1]);
    }

    int getHash(int l, int r)
    {
        auto a = split(root, l - 1);
        auto b = split(a[1], r - l + 1);
        int ret = b[0]->h;
        root = merge(a[0], merge(b[0], b[1]));
        return ret;
    }

    void addChar(int pos, char c)
    {
        auto a = split(root, pos - 1);
        root = merge(merge(a[0], new Node(c - 'a' + 1)), a[1]);
    }

    void print(Node *t)
    {
        if (!t)
            return;
        print(t->c[0]);
        cout << t->h << ' ';
        print(t->c[1]);
    }
};

int rev(int i, int n)
{
    return n - i + 1;
}

signed main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    init();
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    string f = s;
    reverse(f.begin(), f.end());
    Treap a(s), b(f);
    while (q--)
    {
        int op;
        cin >> op;
        if (op == 1)
        {
            int l, r;
            cin >> l >> r;
            int l2 = rev(l, n), r2 = rev(r, n);
            n -= (r - l + 1);
            a.deleteRange(l, r);
            b.deleteRange(r2, l2);
        }
        else if (op == 2)
        {
            n++;
            int pos;
            char c;
            cin >> c >> pos;
            a.addChar(pos, c);
            b.addChar(rev(pos, n), c);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            int l2 = rev(l, n), r2 = rev(r, n);
            if (a.getHash(l, r) == b.getHash(r2, l2))
            {
                cout << "yes\n";
            }
            else
            {
                cout << "no\n";
            }
        }
    }
    return 0;
}
