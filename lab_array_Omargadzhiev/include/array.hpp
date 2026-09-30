#pragma once

// Контейнер "динамический массив с неизменяемым размером".
// Размер задаётся один раз при создании и дальше не меняется
// (нет push_back, resize, insert, erase и т.п.).

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace lab {

template <typename T>
class Array {
public:
    using value_type = T;
    using size_type = std::size_t;
    using iterator = T*;
    using const_iterator = const T*;

    // Пустой массив (размер 0).
    Array() noexcept : size_(0), data_(nullptr) {}

    // Массив из n элементов, значения инициализируются по умолчанию (для чисел - нулями).
    explicit Array(size_type n) : size_(n), data_(n ? new T[n]() : nullptr) {}

    // Массив из n элементов, каждый равен value.
    // Внимание: Array<int>(3, 7) - три семёрки, а Array<int>{3, 7} - два элемента: 3 и 7.
    Array(size_type n, const T& value) : size_(n), data_(n ? new T[n] : nullptr) {
        std::fill(begin(), end(), value);
    }

    // Массив из списка значений: Array<int> a{1, 2, 3};
    Array(std::initializer_list<T> init)
        : size_(init.size()), data_(init.size() ? new T[init.size()] : nullptr) {
        std::copy(init.begin(), init.end(), begin());
    }

    // Копирование (глубокое).
    Array(const Array& other)
        : size_(other.size_), data_(other.size_ ? new T[other.size_] : nullptr) {
        std::copy(other.begin(), other.end(), begin());
    }

    // Перемещение: забираем память у other, other становится пустым.
    Array(Array&& other) noexcept : size_(other.size_), data_(std::move(other.data_)) {
        other.size_ = 0;
    }

    Array& operator=(const Array& other) {
        Array tmp(other);  // copy-and-swap: безопасно при самоприсваивании и исключениях
        swap(tmp);
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }

    ~Array() = default;  // память освобождает unique_ptr<T[]>

    // --- Доступ к элементам ---
    T& operator[](size_type i) { return data_[i]; }              // без проверки границ
    const T& operator[](size_type i) const { return data_[i]; }  // без проверки границ

    T& at(size_type i) {  // с проверкой границ
        check_index(i);
        return data_[i];
    }
    const T& at(size_type i) const {
        check_index(i);
        return data_[i];
    }

    T& front() {
        check_not_empty();
        return data_[0];
    }
    const T& front() const {
        check_not_empty();
        return data_[0];
    }
    T& back() {
        check_not_empty();
        return data_[size_ - 1];
    }
    const T& back() const {
        check_not_empty();
        return data_[size_ - 1];
    }

    T* data() noexcept { return data_.get(); }
    const T* data() const noexcept { return data_.get(); }

    // --- Размер ---
    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    // --- Итераторы ---
    iterator begin() noexcept { return data_.get(); }
    iterator end() noexcept { return data_.get() + size_; }
    const_iterator begin() const noexcept { return data_.get(); }
    const_iterator end() const noexcept { return data_.get() + size_; }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    // --- Прочее ---
    void fill(const T& value) { std::fill(begin(), end(), value); }

    void swap(Array& other) noexcept {
        std::swap(size_, other.size_);
        data_.swap(other.data_);
    }

    friend bool operator==(const Array& a, const Array& b) {
        return a.size_ == b.size_ && std::equal(a.begin(), a.end(), b.begin());
    }
    friend bool operator!=(const Array& a, const Array& b) { return !(a == b); }

private:
    void check_index(size_type i) const {
        if (i >= size_) {
            throw std::out_of_range("Array: index out of range");
        }
    }
    void check_not_empty() const {
        if (size_ == 0) {
            throw std::out_of_range("Array: array is empty");
        }
    }

    size_type size_;
    std::unique_ptr<T[]> data_;
};

}  // namespace lab
