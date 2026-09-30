#pragma once

// Чтение входных данных и вывод результата.

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "algorithms.hpp"
#include "array.hpp"

namespace lab {

// Защита от абсурдных размеров (чтобы не пытаться выделить террабайты памяти).
constexpr long long kMaxArraySize = 100000000LL;

// Формат: первое число - размер n, затем n целых чисел.
inline Array<long long> read_array(std::istream& in) {
    long long n = 0;
    if (!(in >> n)) {
        throw std::runtime_error("cannot read array size");
    }
    if (n < 0) {
        throw std::runtime_error("array size must not be negative");
    }
    if (n > kMaxArraySize) {
        throw std::runtime_error("array size is too large");
    }
    Array<long long> a(static_cast<std::size_t>(n));
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (!(in >> a[i])) {
            throw std::runtime_error("not enough numbers in input (or invalid number)");
        }
    }
    return a;
}

// Направление: left / right (также допустимы l / r, регистр не важен).
inline Direction read_direction(std::istream& in) {
    std::string word;
    if (!(in >> word)) {
        throw std::runtime_error("cannot read shift direction");
    }
    std::transform(word.begin(), word.end(), word.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (word == "left" || word == "l") {
        return Direction::Left;
    }
    if (word == "right" || word == "r") {
        return Direction::Right;
    }
    throw std::runtime_error("unknown direction '" + word + "' (use left or right)");
}

// Число шагов сдвига (целое, не меньше 0).
inline std::size_t read_steps(std::istream& in) {
    long long steps = 0;
    if (!(in >> steps)) {
        throw std::runtime_error("cannot read number of steps");
    }
    if (steps < 0) {
        throw std::runtime_error("number of steps must not be negative");
    }
    return static_cast<std::size_t>(steps);
}

// Элементы через пробел, в конце перевод строки.
inline void print_array(std::ostream& out, const Array<long long>& a) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (i != 0) {
            out << ' ';
        }
        out << a[i];
    }
    out << '\n';
}

}  // namespace lab
