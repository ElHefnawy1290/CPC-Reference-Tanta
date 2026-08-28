vector<int> nextGreater(n, n);
for (int i = 0;i < n;i++)
{
    while (!s.empty() && v[i] > v[s.top()])
    {
        nextGreater[s.top()] = i;
        s.pop();
    }
    s.push(i);
}
