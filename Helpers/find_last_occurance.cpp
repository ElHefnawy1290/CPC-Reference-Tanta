find last occurance for char to specific index 
vector<vector<int>> nxt;
vector<int> last;
string s;
int n,k;
void pre() {
    nxt.assign(n+5, vector<int>(k+5, 0));
    last.assign(k+5, n);

    for (int i = n-1; i >= 0; i--) {
        for (int j = 0; j < k; j++) {
            nxt[i][j] = last[j];
        }
        last[s[i] - 'a'] = i;
    }

}
