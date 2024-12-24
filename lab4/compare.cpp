#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cmath>

const int SIZE = 4; // Размер векторов и матриц
const int SPARSE_THRESHOLD = 10; // Порог для разреженности

// Функция для заполнения вектора случайными значениями
void fillVector(std::vector<double>& vec) {
    for (int i = 0; i < SIZE; ++i) {
        vec[i] = (rand() % 100 < SPARSE_THRESHOLD) ? 0 : static_cast<double>(rand() % 100);
    }
}

// Функция для заполнения разреженного вектора
void fillSparseVector(std::unordered_map<int, double>& sparseVec) {
    for (int i = 0; i < SIZE; ++i) {
        double value = (rand() % 100 < SPARSE_THRESHOLD) ? 0 : static_cast<double>(rand() % 100);
        if (value != 0) {
            sparseVec[i] = value;
        }
    }
}

// Функция для сложения векторов
void vectorAddition(const std::vector<double>& vec1, const std::vector<double>& vec2, std::vector<double>& result) {
    for (int i = 0; i < SIZE; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}

void sparseVectorAddition(const std::unordered_map<int, double>& sparseVec1, const std::unordered_map<int, double>& sparseVec2, std::unordered_map<int, double>& result) {
    // Сложение элементов из первого разреженного вектора
    for (const auto& [index, value] : sparseVec1) {
        result[index] = value; // Сначала добавляем значение из первого вектора
    }
    
    // Сложение элементов из второго разреженного вектора
    for (const auto& [index, value] : sparseVec2) {
        result[index] += value; // Если индекс уже есть, добавляем значение, если нет - создаем новый элемент
    }
}





// Функция для умножения матриц
void matrixMultiplication(const std::vector<std::vector<double>>& mat1, const std::vector<std::vector<double>>& mat2, std::vector<std::vector<double>>& result) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; ++k) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Функция для умножения разреженных матриц
void sparseMatrixMultiplication(const std::unordered_map<int, std::unordered_map<int, double>>& sparseMat1, const std::unordered_map<int, std::unordered_map<int, double>>& sparseMat2, std::unordered_map<int, std::unordered_map<int, double>>& result) {
    for (const auto& [row, cols] : sparseMat1) {
        for (const auto& [col, value] : cols) {
            for (const auto& [k, val] : sparseMat2.at(col)) {
                result[row][k] += value * val;
            }
        }
    }
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(0)));

    // Обычные векторы
    std::vector<double> vec1(SIZE), vec2(SIZE), vecResult(SIZE);
    fillVector(vec1);
    fillVector(vec2);

    // Разреженные векторы
    std::unordered_map<int, double> sparseVec1, sparseVec2, sparseVecResult;
    fillSparseVector(sparseVec1);
    fillSparseVector(sparseVec2);

    // Сравнение времени сложения векторов
    auto start = std::chrono::high_resolution_clock::now();
    vectorAddition(vec1, vec2, vecResult);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> vectorDuration = end - start;
    std::cout << "Vector addition time: " << vectorDuration.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    sparseVectorAddition(sparseVec1, sparseVec2, sparseVecResult);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sparseVectorDuration = end - start;
    std::cout << "Sparse vector addition time: " << sparseVectorDuration.count() << " seconds\n";

    // Обычные матрицы
    std::vector<std::vector<double>> mat1(SIZE, std::vector<double>(SIZE)), mat2(SIZE, std::vector<double>(SIZE)), matResult(SIZE, std::vector<double>(SIZE));
    for (int i = 0; i < SIZE; ++i) {
        fillVector(mat1[i]);
        fillVector(mat2[i]);
    }

    // Разреженные матрицы
    std::unordered_map<int, std::unordered_map<int, double>> sparseMat1, sparseMat2, sparseMatResult;
    for (int i = 0; i < SIZE; ++i) {
        fillSparseVector(sparseMat1[i]);
        fillSparseVector(sparseMat2[i]);
    }

    // Сравнение времени умножения матриц
    start = std::chrono::high_resolution_clock::now();
    matrixMultiplication(mat1, mat2, matResult);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> matrixDuration = end - start;
    std::cout << "Matrix multiplication time: " << matrixDuration.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    sparseMatrixMultiplication(sparseMat1, sparseMat2, sparseMatResult);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sparseMatrixDuration = end - start;
    std::cout << "Sparse matrix multiplication time: " << sparseMatrixDuration.count() << " seconds\n";

    return 0;
}

