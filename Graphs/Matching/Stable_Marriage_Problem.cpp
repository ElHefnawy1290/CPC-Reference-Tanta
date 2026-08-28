#include <bits/stdc++.h>
using namespace std;

/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: STABLE MARRIAGE (GALE-SHAPLEY)
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Stable Matching", "Preference Lists", "Gale-Shapley".
 * - Classic Scenarios: Matching N men to N women, or N students to N colleges,
 *   based on ranked preference lists. A matching is "stable" if there is no
 *   pair (Man A, Woman B) who both prefer each other over their current partners.
 * - The Magic: The Gale-Shapley algorithm guarantees a stable matching in O(N^2) time.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       StableMarriage sm(N);
 *       sm.set_preferences(men_prefs, women_prefs);
 * - Query:
 *       sm.solve();
 * - Extract Assignments:
 *       // match_proposer[i] contains the receiver assigned to proposer i.
 *       // match_receiver[j] contains the proposer assigned to receiver j.
 *
 * 3. ⚙️ HOW TO ADAPT IT (THE DIALS & SWITCHES)
 * - The Proposer Advantage:
 *   Gale-Shapley strongly favors the group doing the proposing!
 *   If you want the optimal outcome for Group X, pass Group X as the "Proposers"
 *   (the first argument in set_preferences).
 * - Unequal sizes: If Proposers > Receivers, add "dummy" receivers that rank
 *   everyone equally at the bottom.
 */

struct StableMarriage
{
    int n;

    // pref_proposer[i][k] = the k-th preferred receiver by proposer i
    vector<vector<int>> pref_proposer;

    // To make rejection O(1), we invert the receiver's preferences.
    // rank_receiver[j][i] = the rank position (1 to N) of proposer i in receiver j's list.
    // Lower rank number means highly preferred!
    vector<vector<int>> rank_receiver;

    // The current matches. 0 means unassigned.
    vector<int> match_proposer;
    vector<int> match_receiver;

    StableMarriage(int n) : n(n)
    {
        pref_proposer.assign(n + 1, vector<int>(n + 1, 0));
        rank_receiver.assign(n + 1, vector<int>(n + 1, 0));
        match_proposer.assign(n + 1, 0);
        match_receiver.assign(n + 1, 0);
    }

    // Both 2D vectors should be 1-indexed: [1..n][1..n]
    void set_preferences(const vector<vector<int>> &proposers, const vector<vector<int>> &receivers)
    {
        pref_proposer = proposers;

        // Invert the receiver's preference list for O(1) comparison later
        for (int j = 1; j <= n; ++j)
        {
            for (int rank = 1; rank <= n; ++rank)
            {
                int proposer_id = receivers[j][rank];
                rank_receiver[j][proposer_id] = rank;
            }
        }
    }

    void solve()
    {
        queue<int> free_proposers;
        // next_proposal[i] tracks the index of the next receiver that proposer i will ask
        vector<int> next_proposal(n + 1, 1);

        for (int i = 1; i <= n; ++i)
        {
            free_proposers.push(i);
        }

        // Loop until every proposer is matched
        while (!free_proposers.empty())
        {
            int p = free_proposers.front();

            // The receiver this proposer is going to ask next
            int r = pref_proposer[p][next_proposal[p]];
            next_proposal[p]++; // Increment pointer so he doesn't ask her again if rejected

            if (match_receiver[r] == 0)
            {
                // Scenario 1: The receiver is entirely free. She happily accepts!
                match_receiver[r] = p;
                match_proposer[p] = r;
                free_proposers.pop(); // The proposer is no longer free
            }
            else
            {
                // Scenario 2: The receiver is already engaged to someone else (current_p).
                int current_p = match_receiver[r];

                // Does she prefer the new proposer (p) over her current partner (current_p)?
                if (rank_receiver[r][p] < rank_receiver[r][current_p])
                {
                    // She dumps her current partner!
                    match_proposer[current_p] = 0;
                    free_proposers.push(current_p); // The dumped partner is now free and goes back in line

                    // She accepts the new proposer
                    match_receiver[r] = p;
                    match_proposer[p] = r;
                    free_proposers.pop();
                }
                // If she prefers her current partner, she rejects 'p', and 'p' stays in the queue.
            }
        }
    }
};

void solve_example()
{
    int n;
    // Read N pairs
    if (!(cin >> n))
        return;

    // 1-indexed preference lists
    vector<vector<int>> men_prefs(n + 1, vector<int>(n + 1));
    vector<vector<int>> women_prefs(n + 1, vector<int>(n + 1));

    // Read Men's preferences (Proposers)
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            cin >> men_prefs[i][j];
        }
    }

    // Read Women's preferences (Receivers)
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            cin >> women_prefs[i][j];
        }
    }

    StableMarriage sm(n);
    sm.set_preferences(men_prefs, women_prefs);
    sm.solve();

    cout << "Stable Matches (Man -> Woman):\n";
    for (int i = 1; i <= n; ++i)
    {
        cout << "Man " << i << " matches with Woman " << sm.match_proposer[i] << "\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // solve_example();
    return 0;
}