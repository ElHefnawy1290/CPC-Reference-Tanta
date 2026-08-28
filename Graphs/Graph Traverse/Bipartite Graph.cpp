// Check Odd Cycle
// using in problems teams or groups or colors
vector<int>  teams;
Graph adj;
int n, m;
bool valid_team = true;
void bfs(int start) {
    queue<int> q;
    q.push(start);
    if (teams[start] == 0)
        teams[start] = 1;
    while (!q.empty())
    {
        int cur = q.front(); q.pop();
        for (auto ch : adj[cur])
        {
            if (!teams[ch])
            {
                q.push(ch);
                teams[ch] = 3 - teams[cur];
            }
            else if (teams[ch] == teams[cur])
            {
                valid_team = false; 
                return;
            }
        }
    }

}