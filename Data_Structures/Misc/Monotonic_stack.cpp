class MonotonicStack
{
public:
    static vector<int> getNextGreater(const vector<int> &nums)
    {
        int n = nums.size();
        vector<int> nextGreater(n, n);
        stack<int> s;
        for (int i = 0; i < n; ++i)
        {
            while (!s.empty() && nums[i] > nums[s.top()])
            {
                nextGreater[s.top()] = i;
                s.pop();
            }
            s.push(i);
        }
        return nextGreater;
    }
};