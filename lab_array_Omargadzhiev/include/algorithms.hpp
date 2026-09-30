#pragma once

// Алгоритмы обработки массива для двух программ лабораторной.

#include <cstddef>

#include "array.hpp"

namespace lab {

// Делится ли число хотя бы на одно из чисел 2..9 (0 делится на всё).
inline bool divisible_by_any_2_to_9(long long value) {
    for (long long k = 2; k <= 9; ++k) {
        if (value % k == 0) {
            return true;
        }
    }
    return false;
}

// Сколько элементов массива кратны хотя бы одному из чисел 2..9.
inline std::size_t count_divisible(const Array<long long>& a) {
    std::size_t count = 0;
    for (long long v : a) {
        if (divisible_by_any_2_to_9(v)) {
            ++count;
        }
    }
    return count;
}

enum class Direction { Left, Right };

// Сдвиг элементов на steps позиций в направлении dir.
// Освободившиеся ячейки заполняются нулями (T{}).
template <typename T>
void shift(Array<T>& a, Direction dir, std::size_t steps) {
    const std::size_t n = a.size();
    if (n == 0 || steps == 0) {
        return;
    }
    if (steps >= n) {  // все элементы "выезжают" за границы
        a.fill(T{});
        return;
    }
    if (dir == Direction::Left) {
        for (std::size_t i = 0; i + steps < n; ++i) {
            a[i] = a[i + steps];
        }
        for (std::size_t i = n - steps; i < n; ++i) {
            a[i] = T{};
        }
    } else {
        for (std::size_t i = n; i > steps; --i) {
            a[i - 1] = a[i - 1 - steps];
        }
        for (std::size_t i = 0; i < steps; ++i) {
            a[i] = T{};
        }
    }
}

}  // namespace lab
