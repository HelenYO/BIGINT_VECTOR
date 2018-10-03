//
// Created by Елена on 06.04.18.
//

#ifndef BIGINTEGER_BIG_INTEGER_H
#define BIGINTEGER_BIG_INTEGER_H

#include <string>
#include <cstdlib>
#include "vector/vector.h"
#include <cstddef>
#include <iosfwd>
#include <cstdint>
#include <functional>

struct big_integer
{
    big_integer();// +
    big_integer(big_integer const& other);// +
    big_integer(int a);// +
    big_integer(uint32_t a);
    explicit big_integer(std::string const& str);// +
    ~big_integer();// +

    big_integer& operator=(big_integer const& other);// +

    big_integer& operator+=(big_integer const& rhs);// +
    big_integer& operator-=(big_integer const& rhs);// +
    big_integer& operator*=(big_integer const& rhs);// +
    big_integer& operator/=(big_integer const& rhs);// +
    big_integer& operator%=(big_integer const& rhs);// +

    big_integer& operator&=(big_integer const& rhs);// +
    big_integer& operator|=(big_integer const& rhs);// +
    big_integer& operator^=(big_integer const& rhs);// +

    big_integer& operator<<=(int rhs);// +
    big_integer& operator>>=(int rhs);// +

    big_integer operator+() const;// +
    big_integer operator-() const;// +
    big_integer operator~() const;// +

    big_integer& operator++();// +
    big_integer operator++(int);// +

    big_integer& operator--();// +
    big_integer operator--(int);// +

    friend bool operator==(big_integer const& a, big_integer const& b);// +
    friend bool operator!=(big_integer const& a, big_integer const& b);// +
    friend bool operator<(big_integer const& a, big_integer const& b);// +
    friend bool operator>(big_integer const& a, big_integer const& b);// +
    friend bool operator<=(big_integer const& a, big_integer const& b);// +
    friend bool operator>=(big_integer const& a, big_integer const& b);// +

    friend std::string to_string(big_integer const& a);// + my strange way


    friend big_integer operator+(big_integer a, big_integer const& b);// +
    friend big_integer operator-(big_integer a, big_integer const& b);// +
    friend big_integer operator*(big_integer a, big_integer const& b);// +
    friend big_integer operator/(big_integer a, big_integer const& b);
    friend big_integer operator%(big_integer a, big_integer const& b);// +

    friend big_integer operator&(big_integer a, big_integer const& b);// +
    friend big_integer operator|(big_integer a, big_integer const& b);// +
    friend big_integer operator^(big_integer a, big_integer const& b);// +

    friend big_integer operator<<(big_integer a, int b);// + ну вроде
    friend big_integer operator>>(big_integer a, int b);// + ну вроде

    friend std::ostream& operator<<(std::ostream& s, big_integer const& a);// +

    static  const uint32_t MAXint32 = 4294967295;

private:
    void resize(size_t expected_size);// ?
    static big_integer do_binary(big_integer const& a, big_integer const& b, char what);// +
    void add(big_integer const& b);//
    void subtract(big_integer const& b);// норм
    void invert();
    void mul(uint32_t val);
    void add(uint32_t val);
    friend big_integer abs(big_integer const &a);
    friend big_integer long_unsigned_division(big_integer const &l, big_integer const &r);
    //std::vector<uint32_t> data;//хранится так: 8 = 0001
    vector data;
    short sign;
};

#endif