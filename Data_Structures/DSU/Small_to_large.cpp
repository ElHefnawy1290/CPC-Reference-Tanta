// Small to large merging

vector<int> colors;
GRAPH graph;

set<int> dfs(int u, int p)
{
    set<int> st{colors[u]};
    for (auto &ch : graph[u])
    {
        if (ch == p)
            continue;
        set<int> child_st = dfs(ch, u);
        if (int(child_st.size()) > int(st.size()))
            swap(st, child_st);
        st.merge(child_st);
    }
    return st;
}
