#pragma once

// Минимальный набор макросов для модульных тестов (без внешних библиотек).

#include <iostream>

namespace testing {
inline int& checks() { static int n = 0; return n; }
inline int& failures() { static int n = 0; return n; }
}  // namespace testing

#define CHECK(cond)                                                              \
    do {                                                                         \
        ++testing::checks();                                                     \
        if (!(cond)) {                                                           \
            ++testing::failures();                                               \
            std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK failed: " #cond \
                      << std::endl;                                              \
        }                                                                        \
    } while (0)

#define CHECK_THROWS(expr, ExType)                                                     \
    do {                                                                               \
        ++testing::checks();                                                           \
        bool caught_ = false;                                                          \
        try {                                                                          \
            (void)(expr);                                                              \
        } catch (const ExType&) {                                                      \
            caught_ = true;                                                            \
        } catch (...) {                                                                \
        }                                                                              \
        if (!caught_) {                                                                \
            ++testing::failures();                                                     \
            std::cerr << __FILE__ << ":" << __LINE__ << ": expected " #ExType " from " \
                      << #expr << std::endl;                                           \
        }                                                                              \
    } while (0)

// Вызывается в конце main: печатает итог и возвращает код выхода.
inline int test_summary(const char* name) {
    std::cout << name << ": " << (testing::checks() - testing::failures()) << "/"
              << testing::checks() << " checks passed" << std::endl;
    return testing::failures() == 0 ? 0 : 1;
}
