vector<int> ans;

class Trie
{
private:
    Trie *child[26];
    int word_cnt{}, prefix_cnt{};

public:
    Trie()
    {
        memset(child, 0, sizeof child);
    }

    void insert(const string &str, int idx = 0)
    {
        prefix_cnt++;
        if (idx == int(str.size()))
        {
            word_cnt++;
            return;
        }
        int curr = str[idx] - 'a';
        if (!child[curr])
            child[curr] = new Trie();
        child[curr]->insert(str, idx + 1);
    }

    void erase(const string &str, int idx = 0)
    {
        if (idx == 0 && !wordSearch(str))
            return;
        prefix_cnt--;
        if (idx == int(str.size()))
        {
            word_cnt--;
            return;
        }
        int curr = str[idx] - 'a';
        child[curr]->erase(str, idx + 1);
    }

    void print(int idx = 0)
    {
        for (int i = 0; i < 26; i++)
        {
            if (!child[i] || !child[i]->prefix_cnt)
                continue;
            for (int k = 0; k < idx; k++)
                cout << " ";
            cout << char(i + 'a') << "\n";
            child[i]->print(idx + 1);
        }
    }

    // count number of chars needed to be typed by the user for each word [using auto-completion]
    // trie.insert("") at the beginning because the user needs to write the first letter
    void count(int cnt = 0)
    {
        if (word_cnt)
            ans.push_back(cnt);
        int num{};
        for (int i = 0; i < 26; i++)
        {
            if (child[i])
                num++;
        }
        for (int i = 0; i < 26; i++)
        {
            if (!child[i] || !child[i]->prefix_cnt)
                continue;
            if (num == 1 && word_cnt == 0)
                child[i]->count(cnt);
            else
                child[i]->count(cnt + 1);
        }
    }

    bool wordSearch(const string &str, int idx = 0)
    {
        if (idx == int(str.size()))
            return word_cnt;
        int curr = str[idx] - 'a';
        if (!child[curr] || child[curr]->prefix_cnt == 0)
            return false;
        return child[curr]->wordSearch(str, idx + 1);
    }

    bool prefixSearch(const string &str, int idx = 0)
    {
        if (idx == int(str.size()))
            return prefix_cnt;
        int curr = str[idx] - 'a';
        if (!child[curr] || child[curr]->prefix_cnt == 0)
            return false;
        return child[curr]->prefixSearch(str, idx + 1);
    }

    int countWordsEqualTo(const string &str, int idx = 0)
    {
        if (idx == int(str.size()))
            return word_cnt;
        int curr = str[idx] - 'a';
        if (!child[curr] || child[curr]->prefix_cnt == 0)
            return 0;
        return child[curr]->countWordsEqualTo(str, idx + 1);
    }

    int countWordsStartingWith(const string &str, int idx = 0)
    {
        if (idx == int(str.size()))
            return prefix_cnt;
        int curr = str[idx] - 'a';
        if (!child[curr] || child[curr]->prefix_cnt == 0)
            return 0;
        return child[curr]->countWordsStartingWith(str, idx + 1);
    }

    void printWordsStartingWith(const string &str, int idx = 0, string res = "")
    {
        if (idx < int(str.size()))
        {
            int curr = str[idx] - 'a';
            if (!child[curr] || child[curr]->prefix_cnt == 0)
                return;
            child[curr]->printWordsStartingWith(str, idx + 1, res + str[idx]);
        }
        else
        {
            if (word_cnt && idx != int(str.size()))
                // ans.push_back(res); //// uncomment this <-----
                for (int i = 0; i < 26; i++)
                {
                    if (!child[i] || child[i]->prefix_cnt == 0)
                        continue;
                    child[i]->printWordsStartingWith(str, idx + 1, res + char('a' + i));
                }
        }
    }

    // longest word with all prefixes problem
    bool checkCompleteness(const string &str, int idx = 0)
    {
        if (idx == int(str.size()))
            return word_cnt > 0;
        int curr = str[idx] - 'a';
        if (!child[curr] || child[curr]->word_cnt == 0)
            return false;
        return child[curr]->checkCompleteness(str, idx + 1);
    }

    string longestCompleteString(vector<string> &v)
    {
        int n = int(v.size());
        string longest{};
        for (int i = 0; i < n; i++)
        {
            if (checkCompleteness(v[i]))
            {
                if (v[i].length() > longest.length() || (v[i].length() == longest.length() && v[i] < longest))
                    longest = v[i];
            }
        }
        return longest.empty() ? "No Complete String" : longest;
    }

    int countNumberOfNodes()
    {
        int ret{};
        for (int i = 0; i < 26; i++)
        {
            if (child[i])
                ret += 1 + child[i]->countNumberOfNodes();
        }
        return ret;
    }
};

// count number of distinct substrings
int countNumberOfDistinctSubStrings(string &str)
{
    int n = int(str.size());
    Trie trie;
    for (int i = 0; i < n; i++)
        trie.insert(str.substr(i));
    return trie.countNumberOfNodes();
}
