/* Task1: Find and implement an optimal representation to store and work on numbers involving a large number of digits. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>
#include <chrono>

int KARATSUBA_BASE_CASE_THRESHOLD = 100;

// First element of vector is MSB, whereas last element of vector is LSB.
template <typename T = int>
std::vector<T> normalize(std::vector<T> digits) {
    reverse(digits.begin(), digits.end());
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

template <typename T = int>
bool is_less(std::vector<T> first, std::vector<T> second) {
    if((int)first.size() < (int)second.size()) {
        return true;
    }
    else if((int)first.size() > (int)second.size()) {
        return false;
    }
    else {
        for(int i = 0; i < (int)first.size(); ++i) {
            if(first[i] < second[i]) {
                return true;
            }
            else if(first[i] > second[i]) {
                return false;
            }
        }
    }
    return false;
}

template <typename T = int>
bool is_more(std::vector<T> first, std::vector<T> second) {
    if((int)first.size() > (int)second.size()) {
        return true;
    }
    else if((int)first.size() < (int)second.size()) {
        return false;
    }
    else {
        for(int i = 0; i < (int)first.size(); ++i) {
            if(first[i] > second[i]) {
                return true;
            }
            else if(first[i] < second[i]) {
                return false;
            }
        }
    }
    return false;
}

template <typename T = int>
bool is_more_or_equal(std::vector<T> first, std::vector<T> second) {
    return (!is_less(first, second));
}

template <typename T = int>
bool is_less_or_equal(std::vector<T> first, std::vector<T> second) {
    return (!is_more(first, second));
}

template <typename T = int>
bool is_equal(std::vector<T> first, std::vector<T> second) {
    return !(is_less(first, second) || is_more(first, second));
}

template <typename T = int>
bool is_not_equal(std::vector<T> first, std::vector<T> second) {
    return !(is_equal(first, second));
}

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
            if (min <= base/2) {
                T remaining = base/2 - min;
                digit = (max - (base - 1)/2) - remaining - 2;
            } else {
                digit = (min - base/2) + (max - (base - 1)/2) - 2;
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

//Returns (a*b)%mod
template <typename T = int>
static T mul_mod(T a, T b, T mod) 
{ 
    T res = 0; // Initialize result 
    a = a % mod; 
    while (b > 0) 
    { 
        // If b is odd, add 'a' to result 
        if (b % 2 == 1) {
            res = res % mod;
            a = a % mod;
            if(res < (mod - a)) {
                res = res + a;
            }
            else {
                T min = std::min(res, a);
                T max = std::max(res, a);
                if (min <= (mod - 1)/2) {
                    T remaining = (mod - 1)/2 - min;
                    res = (max - mod/2) - remaining - 1;
                } else {
                    res = (min - (mod - 1)/2) + (max - mod/2) - 1;
                }
            }
        }

        // Multiply 'a' with 2 
        a = a % mod;
        if(a < mod/2) {
            a = a * 2;
        }
        else {
            a = (a - mod/2) + (a - (mod - 1)/2) - 1;
        } 

        // Divide b by 2 
        b /= 2; 
    } 

    return res % mod; 
}

//Returns (a*b)/mod
template <typename T = int>
static T mult_div(T a, T b, T c) {
    T rem = 0;
    T res = (a / c) * b;
    a = a % c;
    // invariant: a_orig * b_orig = (res * c + rem) + a * b
    // a < c, rem < c.
    while (b != 0) {
        if (b & 1) {
            if(rem < (c - a)) {
                rem += a;
            }
            else {
                T min = std::min(rem, a);
                T max = std::max(rem, a);
                if (min <= (c - 1)/2) {
                    T remaining = (c - 1)/2 - min;
                    rem = (max - c/2) - remaining - 1;
                } else {
                    rem = (min - (c - 1)/2) + (max - c/2) - 1;
                }
                res++;
            }
        }
        b /= 2;

        if(a < c/2) {
            a *= 2;
        }
        else {
            a = (a - c/2) + (a - (c - 1)/2) - 1;
            res += b;
        }
    }
    return res;
} 

template <typename T = int>
std::vector<T> standard_multiplication(std::vector<T> first, std::vector<T> second, T base = std::numeric_limits<T>::max()) {
    // will keep the result number in vector in reverse order
    std::vector<T> temp;
    size_t new_size = first.size() + second.size();
    temp.assign(new_size, 0);

    // Below two indexes are used to find positions
    // in result.
    auto i_n1 = (int) temp.size() - 1;
    // Go from right to left in lhs
    for (int i = (int)first.size()-1; i >= 0; i--) {
        T carry = 0;

        // To shift position to left after every
        // multiplication of a digit in rhs
        int i_n2 = 0;

        // Go from right to left in rhs
        for (int j = (int)second.size()-1; j>=0; j--) {

            // Multiply current digit of second number with current digit of first number
            // and add result to previously stored result at current position.
            T rem = mul_mod(first[i], second[j], base);  
            T rem_s;
            T q = mult_div(first[i], second[j], base);  
            if ( temp[i_n1 - i_n2] >= base - carry ) {
                rem_s = carry - (base - temp[i_n1 - i_n2]);
                ++q;
            }
            else
                rem_s = temp[i_n1 - i_n2] + carry;
            if ( rem >= base - rem_s ) {
                rem -= (base - rem_s);
                ++q;
            }
            else
                rem += rem_s;

            // Carry for next iteration
            carry = q;

            // Store result
            temp[i_n1 - i_n2] = rem;

            i_n2++;
        }

        // store carry in next cell
        if (carry > 0) {
            temp[i_n1 - i_n2] += carry;
        }

        // To shift position to left after every
        // multiplication of a digit in lhs.
        i_n1--;
    }
    temp = normalize(temp);
    return temp;
}

template <typename T = int>
std::vector<T> karatsuba_multiplication (
        std::vector<T> first,
        std::vector<T> second, 
        const T base = std::numeric_limits<T>::max()
) {

    // first --- a, second --- b
    const int a_size = first.size();
    const int b_size = second.size();
    const int max_length = std::max(a_size, b_size);

    if (max_length <= KARATSUBA_BASE_CASE_THRESHOLD || std::abs(a_size - b_size) > std::min(a_size, b_size)) {
        return standard_multiplication(first, second, base);
    }

    // appending zeroes in front to make sizes of a & b equal
    int a_pref_zeroes = 0, b_pref_zeroes = 0;
    if (a_size < max_length) {
        a_pref_zeroes = max_length - a_size;
    } else if (b_size < max_length) {
        b_pref_zeroes = max_length - b_size;
    } 

    const int left_half_length = max_length / 2;
    const int right_half_length = max_length - left_half_length;

    /*
            Variable Explanation
        a is vector representation of "this", b is vector representation of "other"
        a = exact_al * base^(right_half_length) + exact_ar
        b = exact_bl * base^(right_half_length) + exact_br
    */
    std::vector<T> exact_al;
    std::vector<T> exact_ar;
    std::vector<T> exact_bl;
    std::vector<T> exact_br;

    if (a_pref_zeroes > 0) {
        if (a_pref_zeroes >= left_half_length) {
            exact_al = std::vector<T> (1, 0);
            exact_ar = first;
        } else {
            exact_al = std::vector<T> (first.begin(), first.begin() + left_half_length - a_pref_zeroes);
            exact_ar = std::vector<T> (first.begin() + left_half_length - a_pref_zeroes, first.end());
        }
        exact_bl = std::vector<T> (second.begin(), second.begin() + left_half_length);
        exact_br = std::vector<T> (second.begin() + left_half_length, second.end());
    } else if (b_pref_zeroes > 0) {
        if (b_pref_zeroes >= left_half_length) {
            exact_bl = std::vector<T> (1, 0);
            exact_br = second;
        } else {
            exact_bl = std::vector<T> (second.begin(), second.begin() + left_half_length - b_pref_zeroes);
            exact_br = std::vector<T> (second.begin() + left_half_length - b_pref_zeroes, second.end());
        }
        exact_al = std::vector<T> (first.begin(), first.begin() + left_half_length);
        exact_ar = std::vector<T> (first.begin() + left_half_length, first.end());
    } else {
        exact_al = std::vector<T> (first.begin(), first.begin() + left_half_length);
        exact_ar = std::vector<T> (first.begin() + left_half_length, first.end());
        exact_bl = std::vector<T> (second.begin(), second.begin() + left_half_length);
        exact_br = std::vector<T> (second.begin() + left_half_length, second.end());
    }

    exact_al = normalize(exact_al);
    exact_bl = normalize(exact_bl);
    exact_ar = normalize(exact_ar);
    exact_br = normalize(exact_br);

    /*
        Variable explanation
        sum_al_ar = al - ar
        sum_bl_br = br - bl
    */
    int sign = 1;
    std::vector<T> sum_al_ar, sum_bl_br;

    if(is_more(exact_al, exact_ar)) {
        sum_al_ar = subtract_vector(exact_al, exact_ar, base - 1);
    }
    else {
        sum_al_ar = subtract_vector(exact_ar, exact_al, base - 1);
        sign *= -1;
    }
    
    if(is_more(exact_br, exact_bl)) {
        sum_bl_br = subtract_vector(exact_br, exact_bl, base - 1);
    }
    else {
        sum_bl_br = subtract_vector(exact_bl, exact_br, base - 1);
        sign *= -1;
    } 

    exact_al = karatsuba_multiplication(exact_al, exact_bl, base);
    sum_al_ar = karatsuba_multiplication(sum_al_ar, sum_bl_br, base);
    exact_ar = karatsuba_multiplication(exact_ar, exact_br, base);
    /*
        Variable explanation
        exact_al = al * bl
        sum_al_ar = (al - ar) * (br - bl)
        exact_ar = ar * br
    */
    
    std::vector<T> temp = add_vector(exact_al, exact_ar, base - 1);
    if(sign == 1) {
        sum_al_ar = add_vector(sum_al_ar, temp, base - 1);
    }
    else {
        sum_al_ar = subtract_vector(temp, sum_al_ar, base - 1); 
    }
    /*
        sum_al_ar = al * br + ar * bl
    */

    // multiply exact_al by base^(2 * right_half_length)
    for(int i = 0; i < 2 * right_half_length; ++i) {
        exact_al.push_back(0);
    }

    // multiply sum_al_ar by base^(right_half_length)
    for(int i = 0; i < right_half_length; ++i) {
        sum_al_ar.push_back(0);
    }

    exact_al = add_vector(exact_al, sum_al_ar, base - 1);
    exact_al = add_vector(exact_al, exact_ar, base - 1);

    exact_al = normalize(exact_al);
    return exact_al;
}

template <typename T = int>
std::vector<T> multiply_vector(std::vector<T> first, std::vector<T> second, T base = std::numeric_limits<T>::max()) {
    return karatsuba_multiplication(first, second, base);
}

int main()
{
    // Multiplication - 
    std::cout << "Multiplication -" << std::endl;
    std::vector<int> v1, v2, out;

    int max = std::numeric_limits<int>::max();
    v1.push_back(max - 1);
    v2.push_back(2);
    out = multiply_vector(v1, v2, max);
    assert(out.size() == 2);
    std::cout << out[0] << " " << out[1] << std::endl;  // out contains digits : 1, max - 2
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(2);
    v2.push_back(max - 1);
    out = multiply_vector(v1, v2, max);
    assert(out.size() == 2);
    std::cout << out[0] << " " << out[1] << std::endl;  // out contains digits : 1, max - 2
    v1.clear();
    v2.clear();
    out.clear();

    v1.push_back(max - 1);
    v2.push_back(max - 1);
    out = multiply_vector(v1, v2, max);
    assert(out.size() == 2);
    std::cout << out[0] << " " << out[1] << std::endl;  // out contains digits : max - 2, 1
    v1.clear();
    v2.clear();
    out.clear();

    // Vectors of long long int type
    std::vector<long long int> v3, v4, out1;
    long long int max1 = std::numeric_limits<long long int>::max();
    v3.push_back(max1 - 1);
    v4.push_back(max1 - 1);
    out1 = multiply_vector(v3, v4, max1);
    assert(out1.size() == 2);
    std::cout << out1[0] << " " << out1[1] << std::endl;  // out contains digits : max - 2, 1
    v3.clear();
    v4.clear();
    out1.clear();

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(max - 1);
        v2.push_back(max - 1);
    }
    out = multiply_vector(v1, v2, max);
    assert(out.size() == 20);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(max - 1);
        if(i != 0) v2.push_back(max - 1);
    }
    out = multiply_vector(v1, v2, max);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(1);
        v2.push_back(1);
    }
    out = multiply_vector(v1, v2, max);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    for(int i = 0; i < 1000; ++i)
    {
        v1.push_back(max - 1);
        v2.push_back(max - 1);
    }
    auto start = std::chrono::steady_clock::now();
    out = multiply_vector(v1, v2, max);
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Time in standard multiplication: " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    // Testing of Karatsuba Multiplication
    std::cout << "Karatsuba Multiplication -" << std::endl;
    KARATSUBA_BASE_CASE_THRESHOLD = 3;

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(max - 1);
        v2.push_back(max - 1);
    }
    out = multiply_vector(v1, v2, max);
    //assert(out.size() == 20);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(max - 1);
        if(i != 0) v2.push_back(max - 1);
    }
    out = multiply_vector(v1, v2, max);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    // Vectors of long long int type
    for(int i = 0; i < 10; ++i)
    {
        v3.push_back(max1 - 1);
        if(i != 0) v4.push_back(max1 - 1);
    }
    out1 = multiply_vector(v3, v4, max1);
    for(int i = 0; i < (int)out1.size(); ++i)
    {
        std::cout << out1[i] << " ";
    }
    std::cout << std::endl;
    v3.clear();
    v4.clear();
    out1.clear();

    for(int i = 0; i < 10; ++i)
    {
        v1.push_back(1);
        v2.push_back(1);
    }
    out = multiply_vector(v1, v2, max);
    for(int i = 0; i < (int)out.size(); ++i)
    {
        std::cout << out[i] << " ";
    }
    std::cout << std::endl;
    v1.clear();
    v2.clear();
    out.clear();

    for(int i = 0; i < 1000; ++i)
    {
        v1.push_back(max - 1);
        v2.push_back(max - 1);
    }
    auto start1 = std::chrono::steady_clock::now();
    out = multiply_vector(v1, v2, max);
    auto end1 = std::chrono::steady_clock::now();
    auto diff1 = end1 - start1;
    std::cout << "Time in Karatsuba multiplication: " << std::chrono::duration <double, std::milli> (diff1).count() << " ms" << std::endl;
    v1.clear();
    v2.clear();
    out.clear();
}
