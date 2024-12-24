#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cmath>
#include <iterator>

// Шаблонный класс для разреженного вектора
template<typename T>
class SparseVector {
private:
    std::unordered_map<size_t, T> data;
    size_t size;

public:
    explicit SparseVector(size_t size) : size(size) {}

    T get(size_t index) const {
        if (data.count(index)) return data.at(index);
        return 0;
    }

    void set(size_t index, T value) {
        if (index >= size) throw std::out_of_range("Index out of range");
        if (value != 0) data[index] = value;
        else data.erase(index);
    }

    size_t getSize() const { return size; }

    SparseVector<T> operator+(const SparseVector<T>& other) const {
        if (size != other.size) throw std::invalid_argument("Vector sizes do not match");
        SparseVector<T> result(size);
        for (const auto& [index, value] : data)
            result.set(index, value + other.get(index));
        for (const auto& [index, value] : other.data)
            if (!result.data.count(index))
                result.set(index, value);
        return result;
    }

    SparseVector<T> operator*(T scalar) const {
        SparseVector<T> result(size);
        for (const auto& [index, value] : data)
            result.set(index, value * scalar);
        return result;
    }

    T dot(const SparseVector<T>& other) const {
        if (size != other.size) throw std::invalid_argument("Vector sizes do not match");
        T result = 0;
        for (const auto& [index, value] : data)
            result += value * other.get(index);
        return result;
    }

    // Итератор для разреженного вектора
    class Iterator {
    private:
        typename std::unordered_map<size_t, T>::iterator it;
    public:
        Iterator(typename std::unordered_map<size_t, T>::iterator iterator) : it(iterator) {}

        std::pair<size_t, T> operator*() { return *it; }
        Iterator& operator++() { ++it; return *this; }
        bool operator!=(const Iterator& other) const { return it != other.it; }
    };

    Iterator begin() { return Iterator(data.begin()); }
    Iterator end() { return Iterator(data.end()); }
};

// Шаблонный класс для разреженной матрицы
template<typename T>
class SparseMatrix {
private:
    std::unordered_map<size_t, std::unordered_map<size_t, T>> data;
    size_t rows, cols;

public:
    SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {}

    T get(size_t row, size_t col) const {
        if (data.count(row) && data.at(row).count(col))
            return data.at(row).at(col);
        return 0;
    }

    void set(size_t row, size_t col, T value) {
        if (row >= rows || col >= cols) throw std::out_of_range("Index out of range");
        if (value != 0) data[row][col] = value;
        else if (data.count(row)) data[row].erase(col);
    }

    SparseMatrix<T> transpose() const {
        SparseMatrix<T> result(cols, rows);
        for (const auto& [row, cols] : data)
            for (const auto& [col, value] : cols)
                result.set(col, row, value);
        return result;
    }

    SparseMatrix<T> operator+(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) throw std::invalid_argument("Matrix sizes do not match");
        SparseMatrix<T> result(rows, cols);
        for (const auto& [row, cols] : data)
            for (const auto& [col, value] : cols)
                result.set(row, col, value + other.get(row, col));
        for (const auto& [row, cols] : other.data)
            for (const auto& [col, value] : cols)
                if (!result.data[row].count(col))
                    result.set(row, col, value);
        return result;
    }

    SparseMatrix<T> operator*(T scalar) const {
        SparseMatrix<T> result(rows, cols);
        for (const auto& [row, cols] : data)
            for (const auto& [col, value] : cols)
                result.set(row, col, value * scalar);
        return result;
    }

    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j)
                std::cout << get(i, j) << " ";
            std::cout << "\n";
        }
    }
};

int main() {
    // Пример использования разреженного вектора
    SparseVector<double> vec1(5);
    vec1.set(0, 1.0);
    vec1.set(3, 2.0);
    std::cout << "Vector 1: ";
    for (size_t i = 0; i < 5; ++i)
        std::cout << vec1.get(i) << " ";
    std::cout << "\n";

    SparseVector<double> vec2(5);
    vec2.set(1, 3.0);
    vec2.set(3, 4.0);
    std::cout << "Vector 2: ";
    for (size_t i = 0; i < 5; ++i)
        std::cout << vec2.get(i) << " ";
    std::cout << "\n";

    SparseVector<double> vecSum = vec1 + vec2;
    std::cout << "Vector Sum: ";
    for (size_t i = 0; i < 5; ++i)
        std::cout << vecSum.get(i) << " ";
    std::cout << "\n";

    SparseVector<double> vecProd = vec1 * 2;
    std::cout << "Vector Product with Scalar: ";
    for (size_t i = 0; i < 5; ++i)
        std::cout << vecProd.get(i) << " ";
    std::cout << "\n";

    // Пример использования разреженной матрицы
    SparseMatrix<double> mat1(3, 3);
    mat1.set(0, 0, 1.0);
    mat1.set(1, 2, 2.0);
    std::cout << "Matrix 1:\n";
    mat1.print();

    SparseMatrix<double> mat2(3, 3);
    mat2.set(0, 1, 3.0);
    mat2.set(1, 2, 4.0);
    std::cout << "Matrix 2:\n";
    mat2.print();

    SparseMatrix<double> matSum = mat1 + mat2;
    std::cout << "Matrix Sum:\n";
    matSum.print();

    SparseMatrix<double> matProd = mat1 * 2.0;
    std::cout << "Matrix Product with Scalar:\n";
    matProd.print();
    
    SparseMatrix<double> matT = mat1.transpose();
    matT.print();

    return 0;
}

