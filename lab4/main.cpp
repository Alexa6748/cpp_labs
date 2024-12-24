#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cmath>
#include <iterator>
#include <vector>
#include <chrono>

// Шаблонный класс для разреженного вектора
template <typename T>
class SparseVector
{
private:
    std::unordered_map<size_t, T> data;
    size_t size;

public:
    explicit SparseVector(size_t size) : size(size) {}

    T get(size_t index) const
    {
        if (data.count(index))
            return data.at(index);
        return 0;
    }

    void set(size_t index, T value)
    {
        if (index >= size)
            throw std::out_of_range("Index out of range");
        if (value != 0)
            data[index] = value;
        else
            data.erase(index);
    }

    size_t getSize() const { return size; }

    SparseVector<T> operator+(const SparseVector<T> &other) const
    {
        if (size != other.size)
            throw std::invalid_argument("Vector sizes do not match");
        SparseVector<T> result(size);
        for (const auto &[index, value] : data)
            result.set(index, value + other.get(index));
        for (const auto &[index, value] : other.data)
            if (!result.data.count(index))
                result.set(index, value);
        return result;
    }

    SparseVector<T> operator-(const SparseVector<T> &other) const
    {
        if (size != other.size)
            throw std::invalid_argument("Vector sizes do not match");
        SparseVector<T> result(size);
        for (const auto &[index, value] : data)
            result.set(index, value - other.get(index));
        return result;
    }

    SparseVector<T> operator*(T scalar) const
    {
        SparseVector<T> result(size);
        for (const auto &[index, value] : data)
            result.set(index, value * scalar);
        return result;
    }

    T dot(const SparseVector<T> &other) const
    {
        if (size != other.size)
            throw std::invalid_argument("Vector sizes do not match");
        T result = 0;
        for (const auto &[index, value] : data)
            result += value * other.get(index);
        return result;
    }

    // Итератор для разреженного вектора
    class Iterator
    {
    private:
        typename std::unordered_map<size_t, T>::iterator it;

    public:
        Iterator(typename std::unordered_map<size_t, T>::iterator iterator) : it(iterator) {}

        std::pair<size_t, T> operator*() { return *it; }
        Iterator &operator++()
        {
            ++it;
            return *this;
        }
        bool operator!=(const Iterator &other) const { return it != other.it; }
    };

    Iterator begin() { return Iterator(data.begin()); }
    Iterator end() { return Iterator(data.end()); }

    // Поэлементное умножение на скаляр
    SparseVector<T> elementWiseMultiply(T scalar) const
    {
        SparseVector<T> result(size);
        for (const auto &[index, value] : data)
            result.set(index, value * scalar);
        return result;
    }

    // Поэлементное возведение в степень
    SparseVector<T> power(T exponent) const
    {
        SparseVector<T> result(size);
        for (const auto &[index, value] : data)
            result.set(index, std::pow(value, exponent));
        return result;
    }
    void print() const
    {
        for (size_t i = 0; i < size; ++i)
            std::cout << get(i) << " ";
        std::cout << "\n";
    }
};

// Шаблонный класс для разреженной матрицы
template <typename T>
class SparseMatrix
{
private:
    std::unordered_map<size_t, std::unordered_map<size_t, T>> data;
    size_t rows, cols;

public:
    SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {}

    T get(size_t row, size_t col) const
    {
        if (data.count(row) && data.at(row).count(col))
            return data.at(row).at(col);
        return 0;
    }

    void set(size_t row, size_t col, T value)
    {
        if (row >= rows || col >= cols)
            throw std::out_of_range("Index out of range");
        if (value != 0)
            data[row][col] = value;
        else if (data.count(row))
            data[row].erase(col);
    }

    SparseMatrix<T> transpose() const
    {
        SparseMatrix<T> result(cols, rows);
        for (const auto &[row, cols] : data)
            for (const auto &[col, value] : cols)
                result.set(col, row, value);
        return result;
    }

    SparseMatrix<T> operator+(const SparseMatrix<T> &other) const
    {
        if (rows != other.rows || cols != other.cols)
            throw std::invalid_argument("Matrix sizes do not match");
        SparseMatrix<T> result(rows, cols);
        for (const auto &[row, cols] : data)
            for (const auto &[col, value] : cols)
                result.set(row, col, value + other.get(row, col));
        for (const auto &[row, cols] : other.data)
            for (const auto &[col, value] : cols)
                if (!result.data[row].count(col))
                    result.set(row, col, value);
        return result;
    }

    SparseMatrix<T> operator*(T scalar) const
    {
        SparseMatrix<T> result(rows, cols);
        for (const auto &[row, cols] : data)
            for (const auto &[col, value] : cols)
                result.set(row, col, value * scalar);
        return result;
    }

    SparseMatrix<T> operator*(const SparseMatrix<T> &other) const
    {
        if (cols != other.rows)
            throw std::invalid_argument("Matrix dimensions do not allow multiplication");
        SparseMatrix<T> result(rows, other.cols);
        for (const auto &[row, cols] : data)
        {
            for (const auto &[col, value] : cols)
            {
                for (size_t k = 0; k < other.cols; ++k)
                    result.set(row, k, result.get(row, k) + value * other.get(col, k));
            }
        }
        return result;
    }

    SparseVector<T> operator*(const SparseVector<T> &vec) const
    {
        if (cols != vec.getSize())
            throw std::invalid_argument("Matrix and vector dimensions do not match");
        SparseVector<T> result(vec.getSize());
        for (const auto &[row, cols] : data)
        {
            for (const auto &[col, value] : cols)
            {
                result.set(row, result.get(row) + value * vec.get(col));
            }
        }
        return result;
    }

    SparseMatrix<T> inverse() const
    {
        if (rows != cols)
            throw std::invalid_argument("Matrix must be square");

        // Для матриц размером больше 2x2 можно использовать метод Гаусса или другие методы
        // Здесь мы оставим только пример для 2x2
        if (rows == 2)
        {
            T a = get(0, 0);
            T b = get(0, 1);
            T c = get(1, 0);
            T d = get(1, 1);
            T det = a * d - b * c;
            if (det == 0)
                throw std::invalid_argument("Matrix is singular and cannot be inverted");
            SparseMatrix<T> inv(2, 2);
            inv.set(0, 0, d / det);
            inv.set(0, 1, -b / det);
            inv.set(1, 0, -c / det);
            inv.set(1, 1, a / det);
            return inv;
        }

        // Для матриц размером больше 2x2, можно использовать метод Гаусса или другие методы
        // Здесь можно добавить реализацию для больших матриц, если это необходимо
        throw std::invalid_argument("Inverse not implemented for matrices larger than 2x2");
    }

    SparseMatrix<T> power(int exponent) const
    {
        if (rows != cols)
            throw std::invalid_argument("Matrix must be square");
        if (exponent < 0)
            throw std::invalid_argument("Exponent must be non-negative");

        SparseMatrix<T> result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                result.set(i, j, (i == j) ? 1 : 0); // Инициализация единичной матрицы
            }
        }
        SparseMatrix<T> base = *this;
        while (exponent > 0)
        {
            if (exponent % 2 == 1)
            {
                result = result * base;
            }
            base = base * base;
            exponent /= 2;
        }
        return result;
    }

    void print() const
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
                std::cout << get(i, j) << " ";
            std::cout << "\n";
        }
    }
};

int main()
{
    // Пример использования разреженного вектора
    SparseVector<double> vec1(3);
    vec1.set(0, 1.0);
    vec1.set(2, 2.0);
    std::cout << "Vector 1: ";
    vec1.print();

    SparseVector<double> vec2(3);
    vec2.set(1, 3.0);
    vec2.set(2, 4.0);
    std::cout << "Vector 2: ";
    vec2.print();

    SparseVector<double> vecSum = vec1 + vec2;
    std::cout << "Vector Sum: ";
    vecSum.print();

    SparseVector<double> vecMin = vec1 - vec2;
    std::cout << "Vector Minus: ";
    vecMin.print();

    SparseVector<double> vecProd = vec1 * 5;
    std::cout << "Vector 1 Product with Scalar 5: ";
    vecProd.print();

    double vec3 = vec1.dot(vec2);
    std::cout << "Vector 1 Product with Vector 2: " << vec3 << "\n";

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

    SparseMatrix<double> matProd = mat2 * 2.0;
    std::cout << "Matrix 2 Product with Scalar 2:\n";
    matProd.print();

    SparseMatrix<double> matT = mat1.transpose();
    std::cout << "Transposed Matrix 1:\n";
    matT.print();

    // Умножение матрицы на вектор
    SparseVector<double> vecResult = mat1 * vec1;
    std::cout << "Matrix 1 * Vector 1:\n";
    for (size_t i = 0; i < 3; ++i)
        std::cout << vecResult.get(i) << " ";
    std::cout << "\n";

    // Умножение матрицы на матрицу
    SparseMatrix<double> matResult = mat1 * mat2;
    std::cout << "Matrix 1 * Matrix 2:\n";
    matResult.print();

    // Возведение матрицы в степень
    SparseMatrix<double> matPower = mat1.power(2);
    std::cout << "Matrix 1 to the power of 2:\n";
    matPower.print();

    SparseMatrix<double> mat3(2, 2);
    mat3.set(0, 0, 1.0);
    mat3.set(1, 1, 2.0);
    std::cout << "Matrix 3:\n";
    mat3.print();

    // Нахождение обратной матрицы
    SparseMatrix<double> matInv = mat3.inverse();
    std::cout << "Inverse of Matrix 3:\n";
    matInv.print();

    return 0;
}
