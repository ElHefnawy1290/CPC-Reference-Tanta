void PratialSum()
{
    int n; cin >> n;
    vector<int>arr(n + 1);
    vector<ll> pratial(n + 1, 0);
    
    for (int i = 1; i <= n; i++) cin >> arr[i];
    
    int l, r, value; cin >> l >> r >> value;   // sure l < r

    pratial[l] += value;
    pratial[r + 1] -= value;

    for (int i = 1; i <= n; i++)
        pratial[i] += pratial[i - 1];

    for (int i = 1; i <= n; i++)
        pratial[i] += arr[i];

    for (int i = 1; i <= n; i++)
        cout << pratial[i] << "   ";

}
// Function to get the sum of elements in the subarray defined by (x1, y1) to (x2, y2)
long long getSum(const vector<vector<ll>>& Prefix, int x1, int y1, int x2, int y2) {
    return Prefix[x2][y2] - Prefix[x1 - 1][y2] - Prefix[x2][y1 - 1] + Prefix[x1 - 1][y1 - 1];
}
void PrefixSum_2D() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<ll>> arr(n + 1, vector<ll>(m + 1, 0));
    vector<vector<ll>> Prefix(n + 1, vector<ll>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> arr[i][j];
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            Prefix[i][j] = arr[i][j]
                + Prefix[i - 1][j]
                + Prefix[i][j - 1]
                - Prefix[i - 1][j - 1];
        }
    }
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2; // sure x1 < x2 and y1 < y2
        long long result = getSum(Prefix, x1, y1, x2, y2);
        cout << result << endl;
    }
}

void PratialSum_2D()
{
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<ll>> arr(n + 1, vector<ll>(m + 1, 0));
    vector<vector<ll>> Pratial(n + 2, vector<ll>(m + 2, 0));
    while (q--) {
        int x1, y1, x2, y2 , val;
        cin >> x1 >> y1 >> x2 >> y2 >> val;
        Pratial[x1][y1] += val;
        Pratial[x1][y2 + 1] -= val;
        Pratial[x2 + 1][y1] -= val;
        Pratial[x2+1][y2+1] += val;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            Pratial[i][j] = Pratial[i][j]
                + Pratial[i - 1][j]
                + Pratial[i][j - 1]
                - Pratial[i - 1][j - 1];
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << Pratial[i][j] << " ";

        }
        cout << '\n';
    }
     
}
