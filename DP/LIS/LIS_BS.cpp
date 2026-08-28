vector<int> LIS;
for (int i = 0; i < n; i++)
{
    auto it = lower_bound(LIS.begin(), LIS.end(), ans[i]);
    if (it == LIS.end())
        LIS.push_back(ans[i]);
    else
        *it = ans[i];
}
