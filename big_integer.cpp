//
// Created by Елена on 06.04.18.
//

#include "big_integer.h"
#include <iostream>
#include <algorithm>

//конструктор

//big_integer::big_integer(): data(vector(1, 0)) , sign(0)
//{}

big_integer::big_integer() : data(), sign(0){}

big_integer::big_integer(big_integer const &other) = default;

big_integer::big_integer(int a) {
    if (a == 0) {
        sign = 0;
    } else if (a > 0) {
        sign = 1;
    } else {
        sign = -1;
    }
    data.clear();
    data.push_back((uint32_t) std::abs((int64_t) a));//64, тк последний бит знакового инта может не войти
}

big_integer::big_integer(uint32_t a) {
    if (a == 0) {
        sign = 0;
    } else if (a > 0) {
        sign = 1;
    } else {
        sign = -1;
    }
    data.clear();
    data.push_back((uint32_t) std::abs((int64_t) a));
}

void big_integer::mul(uint32_t val) {
    uint64_t tmp = 0;
    for (size_t i = 0; i < data.size(); i++) {
        tmp += (uint64_t) data[i] * (uint64_t) val;
        data[i] = (uint32_t) (tmp & UINT32_MAX);
        tmp >>= 32;
    }
    while (tmp) {
        data.push_back((uint32_t) (tmp & UINT32_MAX));
        tmp >>= 32;
    }
}

big_integer::big_integer(std::string const &str) {
    data.clear();
    data.push_back(0);
    sign = 0;
    for (char c : str) {
        if (c == '+') {
            sign = 1;
            continue;
        }
        if (c == '-') {
            sign = -1;
            continue;
        }
        if (!sign && c != '0') sign = 1;
        mul(10);

        add((uint32_t) (c - '0'));
    }
    if (sign == -1 && !data.back()) {
        sign = 0;
    }
}

big_integer::~big_integer() {
    data.clear();
    sign = 0;
}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer tmp(other);
    vector::swap(data, tmp.data);
    sign = other.sign;
    //data = other.data;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    return *this = *this + rhs;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this = *this - rhs;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {

    big_integer tmp = *this * rhs;
    *this = tmp;
    //std::cout << "in *= \n";
    return *this;
    //return *this = *this * rhs;

}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    return *this = *this / rhs;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return *this = *this % rhs;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    return *this = *this & rhs;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    return *this = *this | rhs;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    return *this = *this ^ rhs;
}

big_integer &big_integer::operator<<=(int rhs) {
    return *this = *this << rhs;
}

big_integer &big_integer::operator>>=(int rhs) {
    return *this = *this >> rhs;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer answer = *this;
    answer.sign = -answer.sign;
    return answer;
}

big_integer big_integer::operator~() const {
    big_integer answer = *this;
    if (answer.sign < 0) {
        answer.invert();
        answer.add(1);
        answer.sign = 1;
    }
    answer.invert();
    if (answer.data.back() != 0) {
        answer.invert();
        answer.add(1);
        answer.sign = -1;
    }
    answer.resize(0);
    return answer;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer big_integer::operator++(int) {
    big_integer temp = *this;
    *this += 1;
    return temp;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}

big_integer big_integer::operator--(int) {
    big_integer temp = *this;
    *this -= 1;
    return temp;
}

void big_integer::add(
        big_integer const &b) // только для чисел одинакового знака, у них должны быть либо одинаковый размер либо у нашего меньше
{
    uint64_t tmp = 0;
    resize(b.data.size());
    for (size_t i = 0; i < b.data.size(); ++i) {
        tmp += (uint64_t) data[i] + (uint64_t) b.data[i];
        data[i] = (uint32_t) (tmp & UINT32_MAX);
        tmp >>= 32;
    }
    size_t pos = b.data.size();
    while (tmp) {
        resize(pos + 1);
        tmp += (uint64_t) data[pos];
        data[pos] = (uint32_t) (tmp & UINT32_MAX);
        tmp >>= 32;
    }
    resize(0);
}

void big_integer::subtract(big_integer const &b)  // для чисел одинакового знака, наше по моулю должно быть больше
{
    int64_t tmp = 0;
    for (size_t i = 0; i < b.data.size(); ++i) {
        tmp += (int64_t) data[i] - (int64_t) b.data[i];
        if (tmp < 0) {
            data[i] = (uint32_t) (tmp + (int64_t) 4294967296);
            tmp = -1;
        } else {
            data[i] = (uint32_t) (tmp);
            tmp = 0;
        }
    }
    size_t pos = b.data.size();
    while (tmp != 0) {
        tmp += (int64_t) data[pos];
        if (tmp < 0) {
            data[pos] = (uint32_t) (tmp + 4294967296);
            tmp = -1;
        } else {
            data[pos] = (uint32_t) (tmp);
            tmp = 0;
        }
    }
    resize(0);
}


big_integer operator+(big_integer a, big_integer const &b) {
    big_integer res = 0;
    if (a.sign == 0)
        return res = b;
    if (b.sign == 0)
        return res = a;
    if (a.sign == b.sign) {
        res = a;
        res.add(b);
        return res;
    }
    if (a.sign < 0) {
        big_integer tmp = a;
        tmp.sign = -tmp.sign;
        res = b;
        if (tmp > res) {
            tmp.subtract(res);
            res = -tmp;
        } else {
            res.subtract(tmp);
        }
    } else {
        big_integer tmp = b;
        tmp.sign = -tmp.sign;
        res = a;
        if (tmp > res) {
            tmp.subtract(res);
            res = -tmp;
        } else {
            res.subtract(tmp);
        }
    }
    res.resize(0);
    return res;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a + (-b);
}


void big_integer::add(uint32_t val) // suppose : BigInt  >= 0
{
    uint64_t tmp = val;
    for (size_t i = 0; i < data.size(); i++) {
        tmp += (uint64_t) data[i];
        data[i] = (uint32_t) (tmp & UINT32_MAX);
        tmp >>= 32;
        if (!tmp)
            break;
    }
    if (tmp) {
        data.push_back((uint32_t) tmp);
    }
}

big_integer operator*(big_integer a, big_integer const &b) {
    big_integer res = 0;
    if (!a.sign || !b.sign)
        return res;
    res.resize(a.data.size() + b.data.size());
    uint64_t tmp = 0;
    for (size_t i = 0; i < a.data.size(); ++i) {
        for (size_t j = 0; j < b.data.size(); ++j) {
            tmp += (uint64_t) res.data[i + j] + (uint64_t) a.data[i] * (uint64_t) b.data[j];
            res.data[i + j] = tmp & UINT32_MAX;
            tmp >>= 32;
        }
        size_t pos = i + b.data.size();
        while (tmp) {
            tmp += (uint64_t) res.data[pos];
            res.data[pos++] = tmp & UINT32_MAX;
            tmp >>= 32;
        }
    }

    res.resize(0);

    res.sign = a.sign * b.sign;

    //std::cout << res.data.size() << " size in mul\n";
    //std::cout << res.data[0] << " data in res[0]\n";
    return res;
}

big_integer abs(big_integer const &a) {
    big_integer res = a;
    res.sign = a.sign * a.sign;
    return res;
}

big_integer operator/(big_integer a, big_integer const &b) {
    if (b == 0) throw "division by zero";
    short newSign = a.sign * b.sign;

    if (a == 0 || abs(b) > abs(a)) {
        a = 0;
    } else if (!(abs(b) == 1)) {
        if (b.data.size() == 1) {
            uint64_t carry = 0;
            for (size_t i = a.data.size(); i > 0; i--) {
                carry = (carry << 32) + a.data[i - 1];
                a.data[i - 1] = static_cast<uint32_t>(carry / b.data[0]);
                carry %= b.data[0];
            }
            a.resize(0);
        } else {
            a = long_unsigned_division(abs(a), abs(b));
        }
    }
    a.sign = newSign;
    a.resize(0);
    return a;
}

big_integer long_unsigned_division(big_integer const &l, big_integer const &r) {
    uint32_t scaling_factor = static_cast<uint32_t>((static_cast<uint64_t> (UINT32_MAX) + 1) /
                                                    (static_cast<uint64_t>(r.data[r.data.size() - 1]) + 1));

    big_integer a(l * scaling_factor);
    big_integer b(r * scaling_factor);

    big_integer result(0);
    result.resize(a.data.size() - b.data.size() + 1);

    big_integer cur_prefix;
    cur_prefix.sign = 1;//
    cur_prefix.data.resize(b.data.size() - 1);
    for (size_t i = a.data.size() - b.data.size() + 1, index = 0; i < a.data.size(); i++, index++) {
        cur_prefix.data[index] = a.data[i];
    }
    uint64_t b1 = b.data[b.data.size() - 1];
    for (size_t i = result.data.size(); i > 0; i--) {
        //cur_prefix.data.insert(cur_prefix.data.begin(), a.data[i - 1]);
        cur_prefix.data.push_front(a.data[i - 1]);

        uint64_t a2 = cur_prefix.data[cur_prefix.data.size() - 1];
        if (cur_prefix.data.size() > b.data.size()) {
            a2 = (a2 << 32) + cur_prefix.data[cur_prefix.data.size() - 2];
        }
        uint64_t quotient = a2 / b1;
        if (quotient >= UINT32_MAX) quotient = UINT32_MAX;

        big_integer mul_dq = b * static_cast<uint32_t>(quotient);

        while (cur_prefix < mul_dq) {
            quotient--;
            mul_dq -= b;
        }
        result.data[i - 1] = static_cast<uint32_t>(quotient & UINT32_MAX);
        cur_prefix -= mul_dq;
    }
    vector::swap(a.data, result.data);
    a.resize(0);
    return a;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a - (a / b) * b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    big_integer temp = big_integer::do_binary(a, b, '&');
    return temp;
}

big_integer operator|(big_integer a, big_integer const &b) {
    big_integer temp = big_integer::do_binary(a, b, '|');
    return temp;
}

big_integer operator^(big_integer a, big_integer const &b) {
    big_integer temp = big_integer::do_binary(a, b, '^');
    return temp;
}

void big_integer::resize(
        size_t expected_size = 0) {// добавляет в начало нули, чтобы сделать одинакого размера || удаляет лишние нули
    //std::cout<< data.size() << " \n";
    if (expected_size == 0) {
        while (data.size() > 1 && data.back() == 0) {
            data.pop_back();
        }
    } else {
        while (data.size() < expected_size) {
            data.push_back(0);
        }
    }
    //if (data.empty()) {
    if (data.size() == 0) {
        data.push_back(0);
    }
    if (data.back() == 0) {
        sign = 0;
    }
}

void big_integer::invert() {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = ~data[i];
    }
}

big_integer big_integer::do_binary(big_integer const &a1, big_integer const &b1, char what) {
    big_integer a = a1;
    big_integer b = b1;
    size_t n = std::max(a.data.size(), b.data.size());
    size_t m = std::min(a.data.size(), b.data.size());
    vector temp(n, 0);

    if (a.sign < 0) {
        a.invert();
        a.add(1);
    }
    if (b.sign < 0) {
        b.invert();
        b.add(1);
    }

    switch (what) {
        case '&':
            for (size_t i = 0; i < m; i++) {
                temp[i] = a.data[i] & b.data[i];
            }
            break;
        case '|':
            for (size_t i = 0; i < m; i++) {
                temp[i] = a.data[i] | b.data[i];
            }
            break;
        case '^':
            for (size_t i = 0; i < m; i++) {
                temp[i] = a.data[i] ^ b.data[i];
            }
            break;
        default:
            break;
    }

    switch (what) {
        case '&':
            if (m < a.data.size()) {// a more big
                if (b.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] & UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] & 0;
                    }
                }
            } else {
                if (a.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] & UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] & 0;
                    }
                }
            }
            break;
        case '|':
            if (m < a.data.size()) {// a more big
                if (b.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] | UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] | 0;
                    }
                }
            } else {
                if (a.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] | UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] | 0;
                    }
                }
            }
            break;
        case '^':
            if (m < a.data.size()) {// a more big
                if (b.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] ^ UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = a.data[i] ^ 0;
                    }
                }
            } else {
                if (a.sign == -1) {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] ^ UINT32_MAX;
                    }
                } else {
                    for (size_t i = m; i < n; i++) {
                        temp[i] = b.data[i] ^ 0;
                    }
                }
            }
            break;
        default:
            break;
    }

    big_integer res(0);
    res.data = temp;

    //res.data(temp);
    res.sign = -1;
    res.resize(0);
    short tmpAs, tmpBs;
    if (a.sign >= 0) {
        tmpAs = 0;
    } else {
        tmpAs = 1;
    }
    if (b.sign >= 0) {
        tmpBs = 0;
    } else {
        tmpBs = 1;
    }

    short tmpSign;
    switch (what) {
        case '&':
            tmpSign = tmpAs & tmpBs;
            break;
        case '|':
            tmpSign = tmpAs | tmpBs;
            break;
        case '^':
            tmpSign = tmpAs ^ tmpBs;
            break;
        default:
            tmpSign = 0;
            break;
    }
    if (res.sign != 0) {
        if (tmpSign > 0) {
            res.invert();
            res.add(1);
            res.sign = -1;
        } else {
            res.sign = 1;
        }
    }
    return res;
}

big_integer operator<<(big_integer a, int b) {
    if (b < 0) {
        return a >> (-b);
    }
    int fullIndent = b >> 5;
    b &= 31;
    big_integer ans = a;
    uint64_t tmp = 0;
    for (size_t i = 0; i < a.data.size(); i++) {
        tmp |= ((uint64_t) a.data[i] << b);
        ans.data[i] = (uint32_t) (tmp & UINT32_MAX);
        tmp >>= 32;
    }
    if (tmp != 0) {
        ans.data.push_back((uint32_t) fullIndent);
    }
    ans.resize(0);
    big_integer temp(0);
    temp.resize((size_t) fullIndent + 1);
    temp.sign = 1;
    temp.data[temp.data.size() - 1] = 1;
    ans = ans * temp;
    return ans;
}

big_integer operator>>(big_integer a, int b) {

    if (b < 0) {
        return a << (-b);
    }
    int fullIndent = b >> 5;
    b &= 31;
    big_integer ans = a;
    uint32_t carry = 0;
    for (size_t i = a.data.size() - 1;; i--) {
        uint32_t nowDigit = ans.data[i];
        ans.data[i] = (ans.data[i] >> b) + (carry << (32 - b));
        carry = nowDigit & ((1 << b) - 1);
        if (i == 0) {
            break;
        }
    }
    big_integer answer2(0);
    answer2.resize(ans.data.size() - fullIndent);
    for (size_t i = 0; i < answer2.data.size(); i++) {
        answer2.data[i] = ans.data[fullIndent + i];
    }
    answer2.resize(0);
    answer2.sign = a.sign;
    if (answer2.sign == -1) {
        answer2--;
    }
    return answer2;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return (a.sign == b.sign && a.data == b.data) || (a.data.size() == 0 && b.data.size() == 0) ||
           (a.sign == 0 && b.sign == 0);
    //if()
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return (a.sign < b.sign);
    }
    if (a.sign == 0) {
        return false;
    }
    if (a.data.size() < b.data.size()) {
        return (a.sign > 0);
    }
    if (a.data.size() > b.data.size()) {
        return (a.sign < 0);
    }
    for (size_t i = a.data.size() - 1;; i--) {
        if (a.data[i] < b.data[i]) {
            return (a.sign > 0);
        }
        if (a.data[i] > b.data[i]) {
            return (a.sign < 0);
        }
        if (i == 0) {
            break;
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return !(a <= b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return (a < b || a == b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

const uint32_t max_int32 = ((1ll << 32) - 1);
const uint64_t base = (uint64_t) max_int32 + 1ll;
const uint32_t base_mod_ten = (uint32_t) (base % 10);

std::string to_string(big_integer const &a) {
    std::string answer = "";
    big_integer cur = a;
    while (cur.sign != 0) {
        uint32_t mod = 0;
        uint64_t carry = 0, tmp = 0;
        for (size_t i = cur.data.size() - 1;; --i) {
            mod = (uint32_t) ((uint64_t) mod * base_mod_ten + (uint64_t) cur.data[i]) % 10;
            tmp = carry * base + (uint64_t) cur.data[i];
            cur.data[i] = (uint32_t) (tmp / 10ll);
            carry = tmp % 10ll;
            if (!i)
                break;
        }
        cur.resize(0);
        answer.push_back(mod + '0');
    }
    if (a.sign < 0)
        answer.push_back('-');
    std::reverse(answer.begin(), answer.end());
    if (answer.empty())
        answer = "0";
    return answer;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    s << to_string(a);
    return s;
}