/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: WARNSDORFF'S HEURISTIC
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Visit every square", "Knight's Tour", "Hamiltonian path on grid".
 * - Classic Scenarios: Output a valid sequence of knight moves to visit all N x N
 *   cells exactly once without repeating.
 * - The Magic: Backtracking has a massive branching factor and TLEs immediately.
 *   Warnsdorff's Rule states: always prioritize the next valid move that has the
 *   LEAST number of subsequent valid moves. This local greedy choice finds the
 *   global Hamiltonian path almost instantly.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Query:
 *       vector<pair<int, int>> tour;
 *       bool success = knight_tour(START_X, START_Y, BOARD_SIZE, tour);
 */

const int dx[] = {1, 1, 2, 2, -1, -1, -2, -2};
const int dy[] = {2, -2, 1, -1, 2, -2, 1, -1};

int get_degree(int x, int y, int n, const vector<vector<int>> &board)
{
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && ny >= 0 && nx < n && ny < n && board[nx][ny] == -1)
        {
            count++;
        }
    }
    return count;
}

bool solve_kt(int x, int y, int move_i, int n, vector<vector<int>> &board, vector<pair<int, int>> &tour)
{
    board[x][y] = move_i;
    tour.push_back({x, y});

    if (move_i == n * n - 1)
        return true;

    vector<pair<int, int>> next_moves;
    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && ny >= 0 && nx < n && ny < n && board[nx][ny] == -1)
        {
            next_moves.push_back({get_degree(nx, ny, n, board), i});
        }
    }

    // Warnsdorff's Heuristic: Sort by degree ascending
    sort(next_moves.begin(), next_moves.end());

    for (auto move : next_moves)
    {
        int i = move.second;
        int nx = x + dx[i], ny = y + dy[i];
        if (solve_kt(nx, ny, move_i + 1, n, board, tour))
        {
            return true;
        }
    }

    // Backtrack
    board[x][y] = -1;
    tour.pop_back();
    return false;
}

bool knight_tour(int start_x, int start_y, int n, vector<pair<int, int>> &tour)
{
    vector<vector<int>> board(n, vector<int>(n, -1));
    return solve_kt(start_x, start_y, 0, n, board, tour);
}