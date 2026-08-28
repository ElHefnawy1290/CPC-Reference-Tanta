struct BitTrie
{
private:
    BitTrie *child[2];
    int cnt{};

public:
    BitTrie()
    {
        child[0] = child[1] = nullptr;
    }

    ~BitTrie()
    {
        delete child[0];
        delete child[1];
    }

    void insert(int num)
    {
        BitTrie *curr = this;
        for (int i = 31; i >= 0; i--)
        {
            int bit = num >> i & 1;
            if (!curr->child[bit])
                curr->child[bit] = new BitTrie();
            curr = curr->child[bit];
            curr->cnt++;
        }
    }

    void erase(int num)
    {
        BitTrie *curr = this;
        for (int i = 31; i >= 0; i--)
        {
            int bit = num >> i & 1;
            curr = curr->child[bit];
            curr->cnt--;
        }
    }

    int getMaxXor(int num)
    {
        BitTrie *curr = this;
        int maxXor = 0;
        for (int i = 31; i >= 0; i--)
        {
            int need = 1 - (num >> i & 1);
            if (curr->child[need] && curr->child[need]->cnt > 0)
            {
                maxXor |= 1ll << i;
                curr = curr->child[need];
            }
            else
                curr = curr->child[1 - need];
        }
        return maxXor;
    }

    // count Xors with X less than K
    int countXorLessThan(int x, int k)
    {
        int ret{};
        BitTrie *curr = this;
        for (int i = 31; i >= 0; i--)
        {
            if (!curr)
                break;
            int bitX = x >> i & 1;
            int bitK = k >> i & 1;
            if (bitK == 0)
                curr = curr->child[bitX];
            else
            {
                if (curr->child[bitX])
                    ret += curr->child[bitX]->cnt;
                curr = curr->child[1 - bitX];
            }
        }
        return ret;
    }
};

// get the Max Xor of two numbers in the array
int getMaxXorOfTwoNumbersInArray(vector<int> &v)
{
    if (v.empty())
        return 0;
    int n = int(v.size());
    BitTrie trie;
    trie.insert(v[0]);
    int max{};
    for (int i = 1; i < n; i++)
    {
        max = std::max(max, trie.getMaxXor(v[i]));
        trie.insert(v[i]);
    }
    return max;
}

struct Query
{
    int x, a, id;

    bool operator<(const Query &other) const
    {
        return a < other.a;
    }
};

// max xor queries for a number with array with threshold A [The element i pick from array must be <= A]
void getMaxXorOfNumWithArrayLessThanThresholdQueries(vector<int> &v)
{
    int n = int(v.size());
    sort(v.begin(), v.end());
    int q;
    cin >> q;
    vector<Query> queries(q);
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i].x >> queries[i].a;
        queries[i].id = i;
    }
    sort(queries.begin(), queries.end());
    BitTrie trie;
    int idx{};
    vector<int> ans(q);
    for (int i = 0; i < q; i++)
    {
        auto [x, a, id] = queries[i];
        while (idx < n && v[idx] <= a)
            trie.insert(v[idx++]);
        if (idx == 0)
            ans[id] = -1;
        else
            ans[id] = trie.getMaxXor(x);
    }
    for (int i = 0; i < q; i++)
        cout << ans[i] << "\n";
}

// Max xor subarray
int getMaxXorSubarray(vector<int> &a)
{
    int n = int(a.size());
    BitTrie trie;
    trie.insert(0);
    int pre{}, ret{};
    for (int i = 0; i < n; i++)
    {
        pre ^= a[i];
        ret = max(ret, trie.getMaxXor(pre));
        trie.insert(pre);
    }
    return ret;
}

int numberOfSubarraysWithXorLessThanK(vector<int> &a, int k)
{
    int n = int(a.size());
    BitTrie trie;
    trie.insert(0);
    int pre{}, ret{};
    for (int i = 0; i < n; i++)
    {
        pre ^= a[i];
        ret += trie.countXorLessThan(pre, k);
        trie.insert(pre);
    }
    return ret;
}
