vector<vector<int>> dp;
int n1, n2;
string s1, s2;

int LCS(int i1, int i2)
{
    if (i1 >= n1 || i2 >= n2)
        return 0;
    auto &ret = dp[i1][i2];
    if (ret != -1)
        return ret;
    int choice1 = LCS(i1 + 1, i2);
    int choice2 = LCS(i1, i2 + 1);
    int choice3{};
    if (s1[i1] == s2[i2])
        choice3 = 1 + LCS(i1 + 1, i2 + 1);
    return ret = max({choice1, choice2, choice3});
}
