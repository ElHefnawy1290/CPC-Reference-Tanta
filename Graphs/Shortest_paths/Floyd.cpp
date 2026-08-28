int n;
GRAPH graph;

vector<vector<int>> floyd()
{
    vector<vector<int>> dist = graph;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] < OO && dist[k][j] < OO)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    return dist;
}

// real
int n;
GRAPH graph;

vector<vector<double>> floyd()
{
    vector<vector<double>> dist = graph;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] < OO && dist[k][j] < OO)
                {
                    if (dist[i][k] + dist[k][j] < dist[i][j] - DBL_EPSILON)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    return dist;
}

// path
int n;
GRAPH graph;
vector<int> path;
vector<vector<int>> phase;

vector<vector<int>> floyd()
{
    phase.assign(n + 1, vector<int>(n + 1, -1));
    vector<vector<int>> dist = graph;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] < OO && dist[k][j] < OO)
                {
                    if (dist[i][j] > dist[i][k] + dist[k][j])
                    {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                        phase[i][j] = k;
                    }
                }
            }
        }
    }
    return dist;
}

// push the start node manually
void build(int i, int j)
{
    if (phase[i][j] == -1)
    {
        path.push_back(j);
        return;
    }
    int k = phase[i][j];
    build(i, k);
    build(k, j);
}
