int curr{a[0]};
int glob{a[0]};
for (int i = 1; i < n; i++)
{
    curr = max(a[i], curr + a[i]);
    glob = max(curr, glob);
}

//------------------------------------------------------------------//

int mx{}, curr{};
pair<int, int> p{0, 0};
pair<int, int> best{};
for (int i = 0;i < n - 1;i++)
{
    curr += a[i];
    if (curr < 0)
    {
        curr = 0;
        p.first = i + 1;
    }
    p.second = i;
    if (curr > mx)
    {
        best = p;
        mx = curr;
    }
    else if (mx == curr)
    {
        if (best.second - best.first < p.second - p.first)
            best = p;
    }
}

//-------------------------------------------------//

int mx = -OO;

for (int left = 0;left < n;left++)
{
    vector<int> temp(n, 0);

    for (int right = left; right < n; right++)
    {
        for (int i = 0; i < n; i++)
        {
            temp[i] += a[i][right];
        }

        int current_max = temp[0];
        int current_sum = temp[0];

        for (int i = 1; i < n; i++)
        {
            current_sum = max(temp[i], current_sum + temp[i]);
            current_max = max(current_max, current_sum);
        }

        mx = max(mx, current_max);
    }
}
