/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: SEGMENT TREE BEATS
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Range chmin" (A[i] = min(A[i], X)), "Range chmax", "Range Sum with Min/Max updates".
 * - Classic Scenarios: Standard Segment Trees completely fail if you try to perform a range minimum
 *   update (e.g., "for all elements from L to R, replace the element with min(element, X)")
 *   while simultaneously asking for the Range Sum.
 * - The Magic: Segment Tree Beats tracks the *First Maximum* and *Second Maximum* in every node.
 *   If your update X is between the First and Second Max, it only affects the First Max!
 *   It uses this trick to update the sum mathematically in amortized O(log^2 N) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Pass a 0-indexed vector<long long> to the constructor.
 *       vector<long long> a = {1, 5, 2, 8, 3};
 *       SegmentTreeBeats seg(a);
 *
 * - Queries (All 0-indexed, Inclusive [L, R]):
 *       seg.updateMin(L, R, X) : Performs A[i] = min(A[i], X) for i in [L, R].
 *       seg.updateMax(L, R, X) : Performs A[i] = max(A[i], X) for i in [L, R].
 *       seg.updateAdd(L, R, X) : Performs A[i] = A[i] + X for i in [L, R].
 *       seg.updateSet(L, R, X) : Performs A[i] = X for i in [L, R].
 *       seg.querySum(L, R)     : Returns the sum of A[i] in [L, R].
 *       seg.queryMin(L, R)     : Returns the Minimum in [L, R].
 *       seg.queryMax(L, R)     : Returns the Maximum in [L, R].
 *
 * - Complexity:
 *       Time: O(N) to build. Amortized O(log^2 N) per query.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - 1-based indexing? Instead of changing the recursive logic, simply add a dummy element `0`
 *   to the front of your vector before initializing: `v.insert(v.begin(), 0);`. Then query `[1, N]`.
 * - Numbers larger than 10^14? The `INF` value is set to 2 * 10^15. If your problem features numbers
 *   larger than that, change the `const long long INF` variable at the top of the struct.
 * - Need more speed? If the problem ONLY asks for `updateMin` and `querySum`, you can safely delete
 *   all variables related to `mn`, `secMn`, and `mnCnt` to cut memory and execution time in half.
 */

#include <bits/stdc++.h>
using namespace std;

// ! Assumes you are using long long to avoid any overflow bugs.
struct SegmentTreeBeats
{
private:
// ! FIX: Added strict parentheses to prevent operator precedence bugs in math
#define L (2 * node + 1)
#define R (2 * node + 2)
#define mid ((l + r) >> 1)

    // ! FIX: Safely typed INF to avoid macro expansion issues
    const long long INF = 2e15 + 5;

    struct Node
    {
        long long sum, mx, secMx, mxCnt, mn, mnCnt, secMn, lazyAdd, lazySet;

        Node(long long summ, long long mxx, long long secMxx, long long mxCntt,
             long long mnn, long long secMnn, long long mnCntt, long long add = 0, long long set = -1)
            : sum(summ), mx(mxx), secMx(secMxx), mxCnt(mxCntt),
              mn(mnn), secMn(secMnn), mnCnt(mnCntt),
              lazyAdd(add), lazySet(set)
        {
        }
    };

    int sz;
    vector<Node> seg;

    Node merge(Node &a, Node &b)
    {
        Node ret(0, -INF, -INF, 0, INF, INF, 0, 0, -1);
        ret.sum = a.sum + b.sum;

        // ? Merge Maximums
        ret.mx = max(a.mx, b.mx);
        ret.secMx = max(a.secMx, b.secMx);
        if (ret.mx == a.mx)
            ret.mxCnt += a.mxCnt;
        else
            ret.secMx = max(ret.secMx, a.mx);
        if (ret.mx == b.mx)
            ret.mxCnt += b.mxCnt;
        else
            ret.secMx = max(ret.secMx, b.mx);

        // ? Merge Minimums
        ret.mn = min(a.mn, b.mn);
        ret.secMn = min(a.secMn, b.secMn);
        if (ret.mn == a.mn)
            ret.mnCnt += a.mnCnt;
        else
            ret.secMn = min(ret.secMn, a.mn);
        if (ret.mn == b.mn)
            ret.mnCnt += b.mnCnt;
        else
            ret.secMn = min(ret.secMn, b.mn);

        return ret;
    }

    void pushSet(int l, int r, int node, long long val)
    {
        seg[node] = Node(val * (r - l + 1), val, -INF, r - l + 1, val, INF, r - l + 1, 0, val);
    }

    void pushMin(int l, int r, int node, long long mn)
    {
        if (seg[node].mn >= mn)
        {
            pushSet(l, r, node, mn);
            return;
        }
        if (seg[node].mx > mn)
        {
            if (seg[node].secMn == seg[node].mx)
                seg[node].secMn = mn;
            long long val = seg[node].mx - mn;
            seg[node].sum -= val * seg[node].mxCnt;
            seg[node].mx = mn;
        }
    }

    void pushMax(int l, int r, int node, long long mx)
    {
        if (seg[node].mx <= mx)
        {
            pushSet(l, r, node, mx);
            return;
        }
        if (seg[node].mn < mx)
        {
            if (seg[node].secMx == seg[node].mn)
                seg[node].secMx = mx;
            long long val = mx - seg[node].mn;
            seg[node].sum += val * seg[node].mnCnt;
            seg[node].mn = mx;
        }
    }

    void pushAdd(int l, int r, int node, long long val)
    {
        if (seg[node].mx == seg[node].mn)
        {
            pushSet(l, r, node, seg[node].mn + val);
            return;
        }
        seg[node].mx += val;
        if (seg[node].secMx != -INF)
            seg[node].secMx += val;
        seg[node].mn += val;
        if (seg[node].secMn != INF)
            seg[node].secMn += val;
        seg[node].sum += (r - l + 1) * val;
        seg[node].lazyAdd += val;
    }

    void propegate(int l, int r, int node)
    {
        if (l == r)
            return;
        if (seg[node].lazySet != -1)
        {
            pushSet(l, mid, L, seg[node].lazySet);
            pushSet(mid + 1, r, R, seg[node].lazySet);
            seg[node].lazySet = -1;
        }
        else
        {
            pushAdd(l, mid, L, seg[node].lazyAdd);
            pushAdd(mid + 1, r, R, seg[node].lazyAdd);
            seg[node].lazyAdd = 0;
            pushMin(l, mid, L, seg[node].mx);
            pushMin(mid + 1, r, R, seg[node].mx);
            pushMax(l, mid, L, seg[node].mn);
            pushMax(mid + 1, r, R, seg[node].mn);
        }
    }

    void build(int l, int r, int node, vector<long long> &v)
    {
        if (l == r)
        {
            if (l < (int)v.size())
            {
                seg[node] = Node(v[l], v[l], -INF, 1, v[l], INF, 1);
            }
            return;
        }
        build(l, mid, L, v);
        build(mid + 1, r, R, v);
        seg[node] = merge(seg[L], seg[R]);
    }

    void updateAdd(int l, int r, int node, int lx, int rx, long long val)
    {
        if (l > rx || r < lx)
            return;
        if (l >= lx && r <= rx)
        {
            pushAdd(l, r, node, val);
            return;
        }
        propegate(l, r, node);
        updateAdd(l, mid, L, lx, rx, val);
        updateAdd(mid + 1, r, R, lx, rx, val);
        seg[node] = merge(seg[L], seg[R]);
    }

    void updateSet(int l, int r, int node, int lx, int rx, long long val)
    {
        if (l > rx || r < lx)
            return;
        if (l >= lx && r <= rx)
        {
            pushSet(l, r, node, val);
            return;
        }
        propegate(l, r, node);
        updateSet(l, mid, L, lx, rx, val);
        updateSet(mid + 1, r, R, lx, rx, val);
        seg[node] = merge(seg[L], seg[R]);
    }

    void updateMin(int l, int r, int node, int lx, int rx, long long mn)
    {
        // ? Core BEATS logic: If the segment max is already <= mn, do nothing.
        if (r < lx || l > rx || seg[node].mx <= mn)
            return;
        // ? If the update affects the maximums, but NOT the second maximums, we can apply it safely!
        if (l >= lx && r <= rx && mn > seg[node].secMx)
        {
            pushMin(l, r, node, mn);
            return;
        }
        propegate(l, r, node);
        updateMin(l, mid, L, lx, rx, mn);
        updateMin(mid + 1, r, R, lx, rx, mn);
        seg[node] = merge(seg[L], seg[R]);
    }

    void updateMax(int l, int r, int node, int lx, int rx, long long mx)
    {
        if (l > rx || r < lx || mx <= seg[node].mn)
            return;
        if (l >= lx && r <= rx && mx < seg[node].secMn)
        {
            pushMax(l, r, node, mx);
            return;
        }
        propegate(l, r, node);
        updateMax(l, mid, L, lx, rx, mx);
        updateMax(mid + 1, r, R, lx, rx, mx);
        seg[node] = merge(seg[L], seg[R]);
    }

    long long querySum(int l, int r, int node, int lx, int rx)
    {
        if (l > rx || r < lx)
            return 0;
        if (l >= lx && r <= rx)
            return seg[node].sum;
        propegate(l, r, node);
        return querySum(l, mid, L, lx, rx) + querySum(mid + 1, r, R, lx, rx);
    }

    long long queryMin(int l, int r, int node, int lx, int rx)
    {
        if (l > rx || r < lx)
            return INF;
        if (l >= lx && r <= rx)
            return seg[node].mn;
        propegate(l, r, node);
        return min(queryMin(l, mid, L, lx, rx), queryMin(mid + 1, r, R, lx, rx));
    }

    long long queryMax(int l, int r, int node, int lx, int rx)
    {
        if (l > rx || r < lx)
            return -INF;
        if (l >= lx && r <= rx)
            return seg[node].mx;
        propegate(l, r, node);
        return max(queryMax(l, mid, L, lx, rx), queryMax(mid + 1, r, R, lx, rx));
    }

#undef L
#undef R
#undef mid

public:
    SegmentTreeBeats(vector<long long> &v)
    {
        int n = v.size() + 5;
        sz = 1;
        while (sz <= n)
            sz <<= 1;
        seg = vector<Node>(sz << 1, Node(0, -INF, -INF, 0, INF, INF, 0, 0, -1));
        build(0, sz - 1, 0, v);
    }

    void updateAdd(int l, int r, long long val) { updateAdd(0, sz - 1, 0, l, r, val); }
    void updateSet(int l, int r, long long val) { updateSet(0, sz - 1, 0, l, r, val); }
    void updateMin(int l, int r, long long mn) { updateMin(0, sz - 1, 0, l, r, mn); }
    void updateMax(int l, int r, long long mx) { updateMax(0, sz - 1, 0, l, r, mx); }
    long long queryMax(int l, int r) { return queryMax(0, sz - 1, 0, l, r); }
    long long queryMin(int l, int r) { return queryMin(0, sz - 1, 0, l, r); }
    long long querySum(int l, int r) { return querySum(0, sz - 1, 0, l, r); }
};