#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: LEFTIST HEAP (MELDABLE)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Meldable Heaps", "Priority Queue on Trees", "Greedy Tree DP".
 * - Classic Scenarios: You need to maintain a set of values for each node in a tree
 *   and frequently merge them with children. std::priority_queue cannot merge.
 * - The Magic: It keeps the tree "left-heavy" to guarantee that the right spine is
 *   always short (O(log N)). Merging two heaps is just merging their right spines!
 *
 * 2. 📦 HOW TO USE IT
 * - Initialization: Root = nullptr.
 * - Merge: root = merge(root1, root2);
 * - Complexity: O(log N) per merge, push, and pop.
 */

struct Node
{
    int val, npl;
    Node *left, *right;

    Node(int v) : val(v), npl(0), left(nullptr), right(nullptr) {}
};

int getNpl(Node *n)
{
    return n ? n->npl : -1;
}

// The core MELD operation
Node *merge(Node *a, Node *b)
{
    if (!a)
        return b;
    if (!b)
        return a;

    // To maintain a Min-Heap, ensure 'a' has the smaller root
    if (a->val > b->val)
        swap(a, b);

    // ! PERSISTENCE NOTE: If you need a fully persistent heap,
    // uncomment the line below to clone the node before modifying.
    // a = new Node(*a);

    a->right = merge(a->right, b);

    // Maintain Leftist Property: Left NPL >= Right NPL
    if (getNpl(a->left) < getNpl(a->right))
    {
        swap(a->left, a->right);
    }

    a->npl = getNpl(a->right) + 1;
    return a;
}

Node *push(Node *root, int val)
{
    return merge(root, new Node(val));
}

Node *pop(Node *root)
{
    Node *left = root->left;
    Node *right = root->right;
    delete root; // Clean up memory
    return merge(left, right);
}

int top(Node *root)
{
    return root->val;
}

int main()
{
    // Example: Standard Usage
    Node *root = nullptr;
    root = push(root, 10);
    root = push(root, 5);

    cout << "Top: " << top(root) << endl; // 5
    root = pop(root);
    cout << "Top after pop: " << top(root) << endl; // 10

    return 0;
}