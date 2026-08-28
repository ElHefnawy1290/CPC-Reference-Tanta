const int N = 100005;
const int BLOCK_SIZE = 316; // Roughly sqrt(N)

int a[N];
int b[N / BLOCK_SIZE + 5]; // Stores the sum of each block

void update(int i, int val)
{
    int block_idx = i / BLOCK_SIZE;
    b[block_idx] = b[block_idx] - a[i] + val; // Update the block sum
    a[i] = val;                               // Update the actual array
}

int query(int l, int r)
{
    int sum = 0;
    int c_l = l / BLOCK_SIZE;
    int c_r = r / BLOCK_SIZE;

    if (c_l == c_r)
    {
        // They are in the exact same block, just loop normally
        for (int i = l; i <= r; ++i)
            sum += a[i];
    }
    else
    {
        // 1. Left tail
        for (int i = l, end = (c_l + 1) * BLOCK_SIZE - 1; i <= end; ++i)
            sum += a[i];
        // 2. Middle full blocks
        for (int i = c_l + 1; i <= c_r - 1; ++i)
            sum += b[i];
        // 3. Right tail
        for (int i = c_r * BLOCK_SIZE; i <= r; ++i)
            sum += a[i];
    }
    return sum;
}
