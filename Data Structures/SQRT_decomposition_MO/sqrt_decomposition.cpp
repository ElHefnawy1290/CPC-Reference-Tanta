
struct SQRT
{
    vector<int> arr, blk;
    int SQ;
    SQRT(vector<int> &a, int sq = 450)
    {
        SQ = sq;
        arr = a;
        int n = arr.size();
        blk.resize(n / sq + 5);
        for (int i = 0; i < n; i++)
        {
            blk[i / sq] += arr[i];
        }
    }
    void update(int idx, int val)
    {
        int blkNum = idx / SQ;
        blk[blkNum] -= arr[idx];
        arr[idx] = val;
        blk[blkNum] += arr[idx];
    }

    int query(int r)
    {
        int res = 0;
        int blkNum = r / SQ;
        for (int i = 0; i < blkNum; i++)
        {
            res += blk[i];
        }
        for (int i = blkNum * SQ; i <= r; i++)
        {
            res += arr[i];
        }
        return res;
    }

    int query(int l, int r)
    {
        return query(r) - query(l - 1);
    }
    /*
    int query(int l,int r) {
        int res = oo;
        while (l<=r) {
            if (l % SQ == 0 && l+SQ <= r) {
                int blkNum =l/SQ;
             //   res = min(res, blk[blkNum]);
                l+=SQ;
            }
            else {
               // res = min(res,arr[l]);
                l++;
            }
        }
        return res;
    }
    */
};