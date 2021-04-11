/* Task3: Provide a template function “get_vector”, similar to std::get, that given the type X as a parameter, returns editable access to the vector of type X 
in a tuple of vectors. */

#include <tuple>
#include <vector>
#include <cassert>

template <typename T, typename... Ts>
auto& get_vector(std::tuple<std::vector<Ts>...>& tuple) 
{
    return std::get<std::vector<T>>(tuple);
}

template <typename T, typename... Ts>
const auto& get_vector(const std::tuple<std::vector<Ts>...>& tuple) 
{
    return std::get<std::vector<T>>(tuple);
}

int main() 
{
    // Use -std=c++17 for testing
    std::tuple<std::vector<int>,std::vector<float>,std::vector<std::string>> t{std::vector<int>(2, 1), std::vector<float>(3, 0.1), std::vector<std::string>(1, "Divyam")};
    
    auto v1 = get_vector<int>(t);
    assert(v1.size() == 2);
    v1.push_back(5);
    assert(v1.size() == 3);

    auto v2 = get_vector<float>(t);
    assert(v2.size() == 3);
    v2.pop_back();
    assert(v2.size() == 2);

    auto v3 = get_vector<std::string>(t);
    assert(v3.size() == 1);
    assert(v3[0].size() == 6);
    v3[0].push_back('S');
    assert(v3[0] == "DivyamS");
}
