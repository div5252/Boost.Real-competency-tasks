#include <utility>
#include <tuple>
#include <vector>
#include <cassert>

constexpr auto func = 
    [](auto t, int x){
        return std::apply([&x](auto... v){ return std::tuple{std::vector(x, v)...}; }, t);
    };

int main()
{
    // Use -std=c++17 for testing
    std::tuple<int, float, char> t{1, 0.1, 'a'};
    int x = 3;
    std::vector<int> v1;
    v1.push_back(1);
    v1.push_back(1);
    v1.push_back(1);
    std::tuple u1{v1, std::vector<float>(3, 0.1), std::vector<char>(3, 'a')};
    assert(u1 == func(t, x));

    std::vector<float> v2;
    v2.push_back(0.1);
    v2.push_back(0.1);
    v2.push_back(0.1);
    std::tuple u2{std::vector<int>(3, 1), v2, std::vector<char>(3, 'a')};
    assert(u2 == func(t, x));

    std::vector<char> v3;
    v3.push_back('a');
    v3.push_back('a');
    v3.push_back('a');
    std::tuple u3{v1, v2, v3};
    assert(u3 == func(t, x));

    std::vector<char> v4;
    v3.push_back('a');
    v3.push_back('b');
    v3.push_back('a');
    std::tuple u4{v1, v2, v4};
    assert(u4 != func(t, x));
}
