class Monotonic_queue
{
    deque<pair<int, int>> dq;
    int k;

public:
    Monotonic_queue(int k) : k(k) {}

    void push(int idx, int val)
    {
        if (!dq.empty() && dq.front().second <= idx - k)
            dq.pop_front();
        while (!dq.empty() && dq.back().second <= val)
            dq.pop_back();
        dq.push_back({idx, val});
    }

    int max() const
    {
        return dq.front().second;
    }
};