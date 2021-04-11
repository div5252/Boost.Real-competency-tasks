/* Task5: Define a user-defined literal assignable to float that fails compilation, when the value provided, cannot be expressed as
a positive integer power of 0.5 (e.g. 0.5, 0.25, 0.125). */

#include <cassert>

constexpr bool is_power_of_half(long double x)
{
    if(x <= 0 || x >= 1) return false;
    while(x < 1)
    {
        x *= 2;
    }
    if(x == 1) return true;
    return false;
}

constexpr long double operator "" _f(long double x)
{
    assert(is_power_of_half(x));
    return x;
}

int main()
{
    // Use -std=c++17 for testing
    // passing compilation cases
    constexpr long double ld1 = 0.5_f;
    constexpr long double ld2 = 0.25_f;
    constexpr long double ld3 = 0.125_f;
    constexpr long double ld4 = 0.0625_f;
    constexpr long double ld5 = 0.03125_f;

    // failing compilation cases, uncomment the below.
    // constexpr long double ldf1 = 0.0_f;
    // constexpr long double ldf2 = 1.0_f;
    // constexpr long double ldf3 = 0.12501_f;
    // constexpr long double ldf4 = 1.33_f;
    // constexpr long double ldf5 = 0.625_f;
}
