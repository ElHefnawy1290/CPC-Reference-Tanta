class BinaryTrie {
    struct Node {
        int ch[2];
        int frq[2];
        Node() {
            ch[0] = ch[1] = -1;
            frq[0] = frq[1] = 0;
        }
    };
    vector<Node> tree;
public:
    BinaryTrie() {
        tree.emplace_back();
        insert(0);   
    }

    void insert(int n) {
        int cur = 0;
        for (int i = 30; i >= 0; i--) {
            int idx = (n >> i) & 1;

            if (tree[cur].ch[idx] == -1) {
                tree[cur].ch[idx] = tree.size();
                tree.emplace_back();
            }
 
            tree[cur].frq[idx]++;
            cur = tree[cur].ch[idx];
        }
    }
 
    void erase(int n) {
        int cur = 0;
        for (int i = 30; i >= 0; i--) {
            int idx = (n >> i) & 1;
            tree[cur].frq[idx]--;
            cur = tree[cur].ch[idx];
        }
    }

    bool search(int n) {
        int cur = 0;
        for (int i = 30; i >= 0; i--) {
            int idx = (n >> i) & 1;
            if (tree[cur].ch[idx] == -1 || tree[cur].frq[idx] == 0) {
                return false;
            }
            cur = tree[cur].ch[idx];
        }
        return true;
    }

    int maxXor(int x) {
        int cur = 0;
        int res = 0;
        for (int i = 30; i >= 0; i--) {
            int idx = (x >> i) & 1;
            int target = 1 - idx;  
 
            if (tree[cur].ch[target] != -1 && tree[cur].frq[target] > 0) {
                res |= (1 << i);
                cur = tree[cur].ch[target];
            } else {
                cur = tree[cur].ch[idx];
            }
        }
        return res;
    }
 
    void clear() {
        tree.clear();
        tree.emplace_back();
        insert(0);
    }
};


