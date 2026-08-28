class Trie {
    struct Node {
        int ch[26];
        int prefix, end;
        Node() {
            prefix = end = 0;
            memset(ch, -1, sizeof(ch));
        }
    };
    vector<Node> tree;
public:
    Trie() {
        tree.emplace_back();
    }

    void insert(string &s) {
        int cur = 0;  
        for (auto i : s) {
            int idx = i - 'a';
            
            if (tree[cur].ch[idx] == -1) {
                tree[cur].ch[idx] = tree.size();
                tree.emplace_back();
            }
            cur = tree[cur].ch[idx];
            tree[cur].prefix++;
        }
        tree[cur].end++;
    }

    bool search(string &s) {
        int cur = 0;
        for (auto i : s) {
            int idx = i - 'a';
            if (tree[cur].ch[idx] == -1) {
                return false;
            }
            cur = tree[cur].ch[idx];
        }
        return tree[cur].end > 0;
    }

    int countWords(string &s) {
        int cur = 0;
        for (auto i : s) {
            int idx = i - 'a';
            if (tree[cur].ch[idx] == -1) {
                return 0;
            }
            cur = tree[cur].ch[idx];
        }
        return tree[cur].end;
    }

    int countPrefix(string &s) {
        int cur = 0;
        for (auto i : s) {
            int idx = i - 'a';
            if (tree[cur].ch[idx] == -1) {
                return 0;
            }
            cur = tree[cur].ch[idx];
        }
        return tree[cur].prefix;
    }
    
    // DFS in Trie
    void dfs(int u) {
        for (int v = 0; v < 26; v++) {
            if (root[u].ch[v] != -1) {
                int x = root[u].ch[v];
                if (root[u].prefix != root[x].prefix) {
                    ans+=root[x].prefix;
                }
                dfs(x);
            }
        }
    }
 
    void clear() {
        tree.clear();
        tree.emplace_back();
    }
};

