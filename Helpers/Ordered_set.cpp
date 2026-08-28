#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

typedef tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

void myerase(ordered_set &t, int v)
{
    int rank = t.order_of_key(v);
    auto it = t.find_by_order(rank);
    t.erase(it);
}
/*
 *  t.order_of_key(v);           // Returns the NUMBER of elements strictly less than v (the rank)
 *  *t.find_by_order(k);         // Returns the value at index k (0-indexed)
 *  myerase(t, v);               // Safely erases exactly ONE instance of v
*/
