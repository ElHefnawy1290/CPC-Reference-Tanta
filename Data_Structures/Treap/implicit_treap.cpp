/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: IMPLICIT TREAP (CARTESIAN TREE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Dynamic Array", "Range Reverse", "Insert/Delete at index", "Cut and Paste".
 * - Classic Scenarios: You have an array and need to perform operations that a Segment
 *   Tree CANNOT do, such as:
 *      - Insert/Delete elements at specific indices (shifting the whole array).
 *      - Reversing a whole subsegment [L, R] dynamically.
 *      - Moving a block [L, R] to the front or end.
 * - The Magic: An Implicit Treap acts like a balanced BST, but instead of sorting by
 *   a "Key", it sorts by the "Implicit Index" (the size of the left subtree). This allows
 *   us to split and merge arbitrary subarrays in O(log N) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization: Root = nullptr. Use `insert` to populate.
 * - Range Operations:
 *       // Reverse range [L, R] (0-indexed)
 *       reverse_range(root, L, R);
 *       // Query sum of range [L, R]
 *       long long ans = query_sum(root, L, R);
 * - Complexity:
 *       Time: O(log N) per operation.
 *       Space: O(N) memory.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - Need Range Add? Add a `lazy` field to the Node and push it in the `push()` function.
 * - Need Max/Min instead of Sum? Update `update()` and `get_sum()` to maintain
 *   `max`/`min` fields instead of `sum`.
 */

#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int val, priority, size;
    long long sum;
    bool rev; // Flag for range reversal (Lazy Propagation)
    Node *left, *right;

    Node(int v) : val(v), priority(rand()), size(1), sum(v), rev(false), left(nullptr), right(nullptr) {}
};

int get_size(Node *t) { return t ? t->size : 0; }
long long get_sum(Node *t) { return t ? t->sum : 0; }

// Update subtree size and sum based on children
void update(Node *t)
{
    if (!t)
        return;
    t->size = 1 + get_size(t->left) + get_size(t->right);
    t->sum = t->val + get_sum(t->left) + get_sum(t->right);
}

// Push lazy propagation (reverse flag) down to children
void push(Node *t)
{
    if (t && t->rev)
    {
        t->rev = false;
        swap(t->left, t->right);
        if (t->left)
            t->left->rev ^= true;
        if (t->right)
            t->right->rev ^= true;
    }
}

// Split treap t into l (first k nodes) and r (remaining nodes)
void split(Node *t, int k, Node *&l, Node *&r)
{
    if (!t)
    {
        l = r = nullptr;
        return;
    }
    push(t);
    int left_size = get_size(t->left);
    if (left_size < k)
    {
        split(t->right, k - left_size - 1, t->right, r);
        l = t;
    }
    else
    {
        split(t->left, k, l, t->left);
        r = t;
    }
    update(t);
}

// Merge treap l and treap r
Node *merge(Node *l, Node *r)
{
    push(l);
    push(r);
    if (!l || !r)
        return l ? l : r;
    if (l->priority > r->priority)
    {
        l->right = merge(l->right, r);
        update(l);
        return l;
    }
    else
    {
        r->left = merge(l, r->left);
        update(r);
        return r;
    }
}

// Reverse subarray [l, r] (0-indexed)
void reverse_range(Node *&root, int l, int r)
{
    Node *t1, *t2, *t3;
    split(root, r + 1, t2, t3);
    split(t2, l, t1, t2);
    if (t2)
        t2->rev ^= true;
    root = merge(t1, merge(t2, t3));
}

// Query sum in subarray [l, r]
long long query_sum(Node *&root, int l, int r)
{
    Node *t1, *t2, *t3;
    split(root, r + 1, t2, t3);
    split(t2, l, t1, t2);
    long long res = get_sum(t2);
    root = merge(t1, merge(t2, t3));
    return res;
}

// Insert value at index pos
void insert(Node *&root, int pos, int val)
{
    Node *t1, *t2;
    split(root, pos, t1, t2);
    root = merge(merge(t1, new Node(val)), t2);
}

int main()
{
    srand(time(0));
    Node *root = nullptr;
    // Example: Insert values
    insert(root, 0, 10);
    insert(root, 1, 20);
    insert(root, 2, 30);

    // Example: Sum query [0, 1]
    // cout << query_sum(root, 0, 1) << endl;

    return 0;
}