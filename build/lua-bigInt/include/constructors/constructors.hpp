/*
    ===========================================================================
    Constructors
    ===========================================================================
*/

#ifndef BIG_INT_CONSTRUCTORS_HPP
#define BIG_INT_CONSTRUCTORS_HPP

#include "../BigInt.hpp"
#include "../functions/utility.hpp"


/*
    Default constructor
    -------------------
*/

BigInt::BigInt() {
    value = "0";
    sign = '+';
}


/*
    Copy constructor
    ----------------
*/

BigInt::BigInt(const BigInt& num) {
    value = num.value;
    sign = num.sign;
}


/*
    Integer to BigInt
    -----------------
*/

BigInt::BigInt(const long long& num) {
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}


/*
    String to BigInt
    ----------------
*/

BigInt::BigInt(const std::string& num) {
    if (num[0] == '+' or num[0] == '-') {     // check for sign
        std::string magnitude = num.substr(1);
        if (is_valid_number(magnitude)) {
            value = magnitude;
            sign = num[0];
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    else {      // if no sign is specified
        if (is_valid_number(num)) {
            value = num;
            sign = '+';    // positive by default
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    strip_leading_zeroes(value);
}

void BigInt::Init()
{
    value = "0";
    sign = '+';
}

void BigInt::Init(const BigInt& num)
{
    value = num.value;
    sign = num.sign;
}

void BigInt::Init(const long long& num)
{
    value = std::to_string(std::abs(num));
    if (num < 0)
        sign = '-';
    else
        sign = '+';
}

void BigInt::Init(const std::string& num)
{
    auto pos = num.find('.');
    std::string parse = num;
	if (pos != num.npos)
	{
		parse = num.substr(0, pos);
	}
    if (parse[0] == '+' or parse[0] == '-') {     // check for sign
        std::string magnitude = parse.substr(1);
        if (is_valid_number(magnitude)) {
            value = magnitude;
            sign = parse[0];
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    else {      // if no sign is specified
        if (is_valid_number(parse)) {
            value = parse;
            sign = '+';    // positive by default
        }
        else {
            throw std::invalid_argument("Expected an integer, got \'" + num + "\'");
        }
    }
    strip_leading_zeroes(value);
}

#endif  // BIG_INT_CONSTRUCTORS_HPP
