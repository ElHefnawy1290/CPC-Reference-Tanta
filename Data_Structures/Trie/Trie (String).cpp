class Trie {
    struct Node {
        map<string, int> ch;
        Node() {
        }
    };
    vector<Node>root;
public:
    Trie() {
        root.emplace_back();
    }
    void insert(vector<string> &words) {
        int cur = 0;
        for (auto s : words) {
            if (!root[cur].ch[s]) {
                root[cur].ch[s] = root.size();
                root.emplace_back();
            }
            cur = root[cur].ch[s];
        }
    }
};