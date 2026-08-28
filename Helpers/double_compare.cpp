int dcomp(double a, double b)
{
    return fabs(a - b) < EPS ? 0 : a > b ? 1
                                         : -1;
}
