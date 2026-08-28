vector<int> compress(vector<int> &v)
{
    vector<int> help = v;
    sort(help.begin(), help.end());
    help.erase(unique(help.begin(), help.end()), help.end());
    // 1-based
    // to get original value -> help[compressed value - 1];
    for (auto &val : v)
        val = distance(help.begin(), lower_bound(help.begin(), help.end(), val)) + 1;
    return help;
}
