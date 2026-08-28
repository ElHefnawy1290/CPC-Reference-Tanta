/**
 * 🛠️ THE ULTIMATE CP TEMPLATE BLUEPRINT: DYNAMIC BITSET
 *
 * 1. 🎯 WHAT PROBLEM DOES THIS SOLVE?
 * - Keywords: "Bitset DP with variable size", "Memory Limit Exceeded".
 * - Classic Scenarios: You need the O(N / 64) speedup of std::bitset, but the
 *   required size of the bitset is determined by the input (e.g., sum of array),
 *   meaning you cannot use std::bitset<MAX_SIZE> without hitting Memory Limits.
 * - The Magic: Wraps a std::vector<uint64_t> and manually implements the
 *   left-shift (<<) and bitwise OR (|) operators with hardware-level carry logic.
 *
 * 2. 📦 HOW TO USE IT (THE BLACK BOX)
 * - Initialization:
 *       DynamicBitset dp(total_sum + 1);
 *       dp.set(0);
 * - Transitions:
 *       dp |= (dp << item_weight);
 */

#include <cstdint>

struct DynamicBitset
{
    vector<uint64_t> blocks;
    int bits;

    DynamicBitset(int n = 0)
    {
        bits = n;
        blocks.assign((n + 63) / 64, 0);
    }

    void set(int i)
    {
        if (i < bits)
            blocks[i / 64] |= (1ULL << (i % 64));
    }

    bool test(int i) const
    {
        if (i >= bits)
            return false;
        return (blocks[i / 64] >> (i % 64)) & 1;
    }

    // Crucial for Knapsack / Reachability DP
    DynamicBitset operator<<(int shift) const
    {
        DynamicBitset res(bits + shift);
        int block_shift = shift / 64;
        int bit_shift = shift % 64;
        uint64_t carry = 0;

        for (int i = 0; i < blocks.size(); i++)
        {
            res.blocks[i + block_shift] = (blocks[i] << bit_shift) | carry;
            if (bit_shift > 0)
            {
                carry = blocks[i] >> (64 - bit_shift);
            }
            else
            {
                carry = 0;
            }
        }
        if (carry && (blocks.size() + block_shift < res.blocks.size()))
        {
            res.blocks[blocks.size() + block_shift] = carry;
        }
        // Restore original logical size constraints if necessary
        res.bits = bits + shift;
        return res;
    }

    void operator|=(const DynamicBitset &other)
    {
        for (int i = 0; i < min(blocks.size(), other.blocks.size()); i++)
        {
            blocks[i] |= other.blocks[i];
        }
    }
};