#include "structure.hpp"

// Реализация pair_hash
template <typename T1, typename T2>
std::size_t pair_hash::operator()(const std::pair<T1, T2>& pair) const {
    return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
}

// Реализация методов SparseVector
template <typename T>
void SparseVector<T>::set(size_t index, const T& value) {
    if (value != T{}) {
        data[index] = value;
    } else {
        data.erase(index);
    }
}

template <typename T>
T SparseVector<T>::get(size_t index) const {
    auto it = data.find(index);
    return (it != data.end()) ? it->second : T{};
}

template <typename T>
auto SparseVector<T>::begin() const {
    return data.begin();
}

template <typename T>
auto SparseVector<T>::end() const {
    return data.end();
}

template <typename T>
void SparseVector<T>::print(size_t size) const {
    for (size_t i = 0; i < size; ++i) {
        if (data.count(i)) {
            std::cout << data.at(i) << " ";
        } else {
            std::cout << 0 << " ";
        }
    }
    std::cout << std::endl;
}

// Реализация методов SparseMatrix
template <typename T>
void SparseMatrix<T>::set(size_t row, size_t col, const T& value) {
    if (value != T{}) {
        data[{row, col}] = value;
    } else {
        data.erase({row, col});
    }
}

template <typename T>
T SparseMatrix<T>::get(size_t row, size_t col) const {
    auto it = data.find({row, col});
    return (it != data.end()) ? it->second : T{};
}

template <typename T>
auto SparseMatrix<T>::begin() const {
    return data.begin();
}

template <typename T>
auto SparseMatrix<T>::end() const {
    return data.end();
}

template <typename T>
void SparseMatrix<T>::print(size_t rows, size_t cols) const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << std::setw(5) << get(i, j);
        }
        std::cout << '\n';
    }
}

void try_structure() {
    std::cout << "Example vector length 6:" << std::endl;
    SparseVector<int> vec;
    vec.set(0, 10);
    vec.set(3, 20);
    vec.set(5, 0); // Устанавливаем "нулевое" значение, элемент удаляется

    std::cout << "Sparse Vector:" << std::endl;
    vec.print(6);

    SparseMatrix<double> mat;
    mat.set(0, 0, 1.5);
    mat.set(1, 2, 2.5);
    mat.set(2, 1, 0.0); // "Нулевое" значение, элемент удаляется

    std::cout << "\nExample matrix 3x3:";
    std::cout << "\nSparse Matrix:" << std::endl;
    mat.print(3, 3);
}
