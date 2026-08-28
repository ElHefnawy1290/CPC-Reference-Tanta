int getRandom(int min, int max)
{
    static std::mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distrib(min, max);
    return distrib(gen);
}
