/* Task1: Find and implement an optimal representation to store and work on numbers involving a large number of digits. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>

// First element of vector is MSB, whereas last element of vector is LSB.
template <typename T = int>
std::vector<T> add_vector(std::vector<T> &first, std::vector<T> &second, T base = std::numeric_limits<T>::max()){
    std::vector<T> temp;
    size_t length = std::max((int)first.size(), (int)second.size());
    T carry = 0;

    // we walk the numbers from the lowest to the highest digit
    for (int i = 0; i < length; i++) {

        T lhs_digit = 0;
        if ((int)first.size() - 1 - i >= 0) {
            lhs_digit = first[(int)first.size() - 1 - i];
        }

        T rhs_digit = 0;
        if ((int)second.size() - 1 - i >= 0) {
            rhs_digit = second[(int)second.size() - 1 - i];
        }
        
        T digit;
        T orig_carry = carry;
        carry = 0;
        if ((base - lhs_digit) < rhs_digit) {
            T min = std::min(lhs_digit, rhs_digit);
            T max = std::max(lhs_digit, rhs_digit);
            if (min <= (base - 1)/2) {
                T remaining = (base - 1)/2 - min;
                digit = (max - base/2) - remaining - 2;
            } else {
                digit = (min - (base - 1)/2) + (max - base/2) - 2;
            }
            carry = 1;
        }
        else {
            carry = 0;
            digit = rhs_digit + lhs_digit;
        }
        if (digit < base || orig_carry == 0) {
            digit += orig_carry;
        }
        else {
            carry = 1;
            digit = 0;
        }
        temp.push_back(digit);
    }
    if (carry == 1) {
        temp.push_back(1);
    }
    while(temp.size() > 1 && temp[temp.size() - 1] == 0) {
        temp.pop_back();
    }
    std::reverse(temp.begin(), temp.end());
    return temp;
}

template <typename T = int>
std::vector<T> subtract_vector(std::vector<T> first, std::vector<T> second, T base = std::numeric_limits<T>::max()) {
    std::vector<T> result;
    int length = std::max(first.size(), second.size());
    int borrow = 0;

    // we walk the numbers from the lowest to the highest digit
    for (int i = 0; i < length; i++) {

        T digit = 0;

        T lhs_digit = 0;
        if ((int)first.size() - 1 - i >= 0) {
            lhs_digit = first[(int)first.size() - 1 - i];
        }

        T rhs_digit = 0;
        if ((int)second.size() - 1 - i >= 0) {
            rhs_digit = second[(int)second.size() - 1 - i];
        }

        if (lhs_digit < borrow) {
            digit = (base - rhs_digit) + 1 - borrow;
        } else {
            lhs_digit -= borrow;
            borrow = 0;
            
            if (lhs_digit < rhs_digit) {
            ++borrow;
            digit = (base - (rhs_digit -1)) + lhs_digit;
            } else {
                digit = lhs_digit - rhs_digit;
            }

        }                    
        result.push_back(digit);
    }
    while(result.size() > 1 && result[result.size() - 1] == 0) {
        result.pop_back();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int main()
{
    // Addition
    std::cout << "Addition -" << std::endl;
    std::vector<int> v1, v2, out;

    int max = std::numeric_limits<int>::max();
    v1.push_back(max);
    v2.push_back(max);
    out = add_vector(v1, v2);
    assert(out.size() == 2);
    std::cout << out[0] << " " << out[1] << std::endl;  // out contains digits : 1, max - 1
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(max);
    v2.push_back(0);
    out = add_vector(v1, v2);
    assert(out.size() == 1);
    std::cout << out[0] << std::endl;  // out contains digits : max
    v1.clear();
    v2.clear();
    out.clear();
    
    v1.push_back(max);
    v1.push_back(max);
    v2.push_back(max);
    v2.push_back(max);
    out = add_vector(v1, v2);
    assert(out.size() == 3);
    std::cout << out[0] << " " << out[1] << " " << out[2] << std::endl;  // out contains digits : 1, max, max - 1
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(max);
    v2.push_back(1);
    out = add_vector(v1, v2);
    assert(out.size() == 2);
    std::cout << out[0] << " " << out[1] << std::endl;  // out contains digits : 1, 0
    v1.clear();
    v2.clear();
    out.clear();

    // Vectors of long long int type
    std::vector<long long int> v3, v4, out1;
    long long int max1 = std::numeric_limits<long long int>::max();
    v3.push_back(max1);
    v4.push_back(max1);
    out1 = add_vector(v3, v4);
    assert(out1.size() == 2);
    std::cout << out1[0] << " " << out1[1] << std::endl;  // out contains digits : 1, max - 1
    v3.clear();
    v4.clear();
    out1.clear();

    // Subtraction
    std::cout << "Subtraction -" << std::endl;

    v1.push_back(1);
    v1.push_back(0);
    v2.push_back(max);
    out = subtract_vector(v1, v2);  // out contains digits : 1 
    assert(out.size() == 1);
    std::cout << out[0] << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(max);
    v1.push_back(max);
    v2.push_back(max);
    v2.push_back(max);
    out = subtract_vector(v1, v2);  // out contains digits : 0
    assert(out.size() == 1);
    std::cout << out[0] << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(1);
    v1.push_back(0);
    v2.push_back(1);
    out = subtract_vector(v1, v2);  // out contains digits : max
    assert(out.size() == 1);
    std::cout << out[0] << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    // Vectors of long long int type
    v3.push_back(1);
    v3.push_back(0);
    v4.push_back(1);
    out1 = subtract_vector(v3, v4);  // out contains digits : max
    assert(out1.size() == 1);
    std::cout << out1[0] << std::endl;
    v3.clear();
    v4.clear();
    out1.clear();
}
