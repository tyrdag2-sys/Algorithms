// Модульные тесты контейнера lab::Array.

#include <string>
#include <utility>

#include "array.hpp"
#include "test_framework.hpp"

using lab::Array;

static void test_constructors() {
    Array<int> empty;
    CHECK(empty.size() == 0);
    CHECK(empty.empty());
    CHECK(empty.begin() == empty.end());

    Array<int> zeros(4);  // значения по умолчанию - нули
    CHECK(zeros.size() == 4);
    CHECK(!zeros.empty());
    for (std::size_t i = 0; i < zeros.size(); ++i) {
        CHECK(zeros[i] == 0);
    }

    Array<int> sevens(3, 7);
    CHECK(sevens.size() == 3);
    CHECK(sevens[0] == 7 && sevens[1] == 7 && sevens[2] == 7);

    Array<int> list{3, 7};  // фигурные скобки - список значений
    CHECK(list.size() == 2);
    CHECK(list[0] == 3 && list[1] == 7);

    Array<int> zero_size(0);
    CHECK(zero_size.empty());
}

static void test_element_access() {
    Array<int> a{10, 20, 30};
    CHECK(a[0] == 10 && a[2] == 30);
    a[1] = 25;
    CHECK(a.at(1) == 25);
    CHECK(a.front() == 10);
    CHECK(a.back() == 30);
    a.front() = 1;
    a.back() = 3;
    CHECK(a[0] == 1 && a[2] == 3);
    CHECK(a.data()[1] == 25);

    const Array<int>& c = a;
    CHECK(c[1] == 25);
    CHECK(c.at(2) == 3);
    CHECK(c.front() == 1 && c.back() == 3);

    CHECK_THROWS(a.at(3), std::out_of_range);
    CHECK_THROWS(a.at(1000), std::out_of_range);
    CHECK_THROWS(c.at(3), std::out_of_range);

    Array<int> empty;
    CHECK_THROWS(empty.at(0), std::out_of_range);
    CHECK_THROWS(empty.front(), std::out_of_range);
    CHECK_THROWS(empty.back(), std::out_of_range);
}

static void test_iterators() {
    Array<int> a{1, 2, 3, 4};
    int sum = 0;
    for (int v : a) {
        sum += v;
    }
    CHECK(sum == 10);

    for (int& v : a) {
        v *= 2;
    }
    CHECK(a[0] == 2 && a[3] == 8);
    CHECK(a.end() - a.begin() == 4);
    CHECK(a.cend() - a.cbegin() == 4);
}

static void test_copy_and_move() {
    Array<int> a{1, 2, 3};

    Array<int> b(a);  // копирование - независимая копия
    CHECK(a == b);
    b[0] = 100;
    CHECK(a[0] == 1);
    CHECK(a != b);

    Array<int> c;
    c = a;
    CHECK(c == a);
    c[1] = 200;
    CHECK(a[1] == 2);

    Array<int>& self = c;  // самоприсваивание не должно ломать массив
    c = self;
    CHECK(c.size() == 3 && c[1] == 200);

    Array<int> d(std::move(b));  // перемещение: источник становится пустым
    CHECK(d.size() == 3 && d[0] == 100);
    CHECK(b.size() == 0);

    Array<int> e;
    e = std::move(d);
    CHECK(e.size() == 3 && e[0] == 100);
    CHECK(d.size() == 0);
}

static void test_fill_swap_compare() {
    Array<int> a{1, 2, 3};
    a.fill(9);
    CHECK(a[0] == 9 && a[1] == 9 && a[2] == 9);

    Array<int> x{1, 2};
    Array<int> y{7, 8, 9};
    x.swap(y);
    CHECK(x.size() == 3 && x[0] == 7);
    CHECK(y.size() == 2 && y[1] == 2);

    CHECK(Array<int>({1, 2, 3}) == Array<int>({1, 2, 3}));
    CHECK(Array<int>({1, 2, 3}) != Array<int>({1, 2, 4}));
    CHECK(Array<int>({1, 2}) != Array<int>({1, 2, 3}));
    CHECK(Array<int>() == Array<int>());
}

static void test_other_types() {
    Array<std::string> s{"a", "bb", "ccc"};
    CHECK(s.size() == 3);
    CHECK(s[2] == "ccc");
    Array<std::string> t = s;
    t[0] = "z";
    CHECK(s[0] == "a");

    Array<double> d(2, 1.5);
    CHECK(d[0] == 1.5 && d[1] == 1.5);
}

int main() {
    test_constructors();
    test_element_access();
    test_iterators();
    test_copy_and_move();
    test_fill_swap_compare();
    test_other_types();
    return test_summary("test_array");
}
