// Модульные тесты алгоритмов и чтения входных данных.

#include <sstream>

#include "algorithms.hpp"
#include "array.hpp"
#include "io.hpp"
#include "test_framework.hpp"

using lab::Array;
using lab::Direction;

static void test_divisible() {
    CHECK(!lab::divisible_by_any_2_to_9(1));
    CHECK(!lab::divisible_by_any_2_to_9(11));
    CHECK(!lab::divisible_by_any_2_to_9(13));
    CHECK(lab::divisible_by_any_2_to_9(2));
    CHECK(lab::divisible_by_any_2_to_9(7));
    CHECK(lab::divisible_by_any_2_to_9(9));
    CHECK(lab::divisible_by_any_2_to_9(10));
    CHECK(lab::divisible_by_any_2_to_9(0));
    CHECK(lab::divisible_by_any_2_to_9(-4));
    CHECK(!lab::divisible_by_any_2_to_9(-11));

    Array<long long> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    CHECK(lab::count_divisible(a) == 9);
    Array<long long> b{1, 11, 13, 17, 19};
    CHECK(lab::count_divisible(b) == 0);
    Array<long long> empty;
    CHECK(lab::count_divisible(empty) == 0);
}

static void test_shift() {
    {
        Array<int> a{1, 2, 3, 4, 5};
        lab::shift(a, Direction::Left, 2);
        CHECK((a == Array<int>{3, 4, 5, 0, 0}));
    }
    {
        Array<int> a{1, 2, 3, 4, 5};
        lab::shift(a, Direction::Right, 2);
        CHECK((a == Array<int>{0, 0, 1, 2, 3}));
    }
    {
        Array<int> a{1, 2, 3, 4, 5};
        lab::shift(a, Direction::Left, 1);
        CHECK((a == Array<int>{2, 3, 4, 5, 0}));
    }
    {
        Array<int> a{1, 2, 3, 4, 5};
        lab::shift(a, Direction::Right, 4);
        CHECK((a == Array<int>{0, 0, 0, 0, 1}));
    }
    {
        Array<int> a{1, 2, 3};
        lab::shift(a, Direction::Left, 3);  // шагов ровно столько, сколько элементов
        CHECK((a == Array<int>{0, 0, 0}));
    }
    {
        Array<int> a{1, 2, 3};
        lab::shift(a, Direction::Right, 100);  // шагов больше размера
        CHECK((a == Array<int>{0, 0, 0}));
    }
    {
        Array<int> a{1, 2, 3};
        lab::shift(a, Direction::Left, 0);  // нулевой сдвиг ничего не меняет
        CHECK((a == Array<int>{1, 2, 3}));
    }
    {
        Array<int> empty;
        lab::shift(empty, Direction::Right, 5);  // пустой массив не падает
        CHECK(empty.empty());
    }
}

static void test_reading() {
    {
        std::istringstream in("3\n10 20 30\n");
        Array<long long> a = lab::read_array(in);
        CHECK(a.size() == 3 && a[0] == 10 && a[2] == 30);
    }
    {
        std::istringstream in("0\n");
        CHECK(lab::read_array(in).empty());
    }
    {
        std::istringstream in("5\n1 2 3\n");  // чисел меньше, чем заявлено
        CHECK_THROWS(lab::read_array(in), std::runtime_error);
    }
    {
        std::istringstream in("-1\n");
        CHECK_THROWS(lab::read_array(in), std::runtime_error);
    }
    {
        std::istringstream in("abc\n");
        CHECK_THROWS(lab::read_array(in), std::runtime_error);
    }
    {
        std::istringstream in("2\n1 x\n");
        CHECK_THROWS(lab::read_array(in), std::runtime_error);
    }
    {
        std::istringstream in("999999999999\n");  // слишком большой размер
        CHECK_THROWS(lab::read_array(in), std::runtime_error);
    }

    {
        std::istringstream in("left Right L r");
        CHECK(lab::read_direction(in) == Direction::Left);
        CHECK(lab::read_direction(in) == Direction::Right);
        CHECK(lab::read_direction(in) == Direction::Left);
        CHECK(lab::read_direction(in) == Direction::Right);
    }
    {
        std::istringstream in("up");
        CHECK_THROWS(lab::read_direction(in), std::runtime_error);
    }
    {
        std::istringstream in("");
        CHECK_THROWS(lab::read_direction(in), std::runtime_error);
    }
    {
        std::istringstream in("7 -2 q");
        CHECK(lab::read_steps(in) == 7);
        CHECK_THROWS(lab::read_steps(in), std::runtime_error);  // отрицательное
        CHECK_THROWS(lab::read_steps(in), std::runtime_error);  // не число
    }
}

static void test_printing() {
    std::ostringstream out;
    lab::print_array(out, Array<long long>{1, -2, 3});
    CHECK(out.str() == "1 -2 3\n");

    std::ostringstream out_empty;
    lab::print_array(out_empty, Array<long long>{});
    CHECK(out_empty.str() == "\n");
}

int main() {
    test_divisible();
    test_shift();
    test_reading();
    test_printing();
    return test_summary("test_algorithms");
}
