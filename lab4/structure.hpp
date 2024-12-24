#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <utility>
#include <functional>

// Хеш-функция для std::pair
struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const;
};

// Шаблонный класс для разреженного вектора
template <typename T>
class SparseVector {
private:
    std::unordered_map<size_t, T> data;

public:
    void set(size_t index, const T& value);
    T get(size_t index) const;
    auto begin() const;
    auto end() const;
    void print(size_t size) const;
};

// Шаблонный класс для разреженной матрицы
template <typename T>
class SparseMatrix {
private:
    std::unordered_map<std::pair<size_t, size_t>, T, pair_hash> data;

public:
    void set(size_t row, size_t col, const T& value);
    T get(size_t row, size_t col) const;
    auto begin() const;
    auto end() const;
    void print(size_t rows, size_t cols) const;
};

void try_structure();

#endif // STRUCTURE_HPP
