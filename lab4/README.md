# Решение

## Создание шаблона класса и методов для работы с вектором

```cpp
template <typename T>
class SparseVector <...>
```

#### Члены класса

```cpp
private:
    std::unordered_map<size_t, T> data;
    size_t size;
```
Здесь объявляются два приватных члена класса:
- `std::unordered_map<size_t, T> data`: контейнер типа `std::unordered_map`, который хранит пары ключ-значение. Ключи имеют тип `size_t` (целое число без знака), а значения — тип `T` (задан пользователем).
- `size_t size`: переменная, содержащая размер вектора.

#### Конструктор

```cpp
public:
    explicit SparseVector(size_t size) : size(size) {}
```
Конструктор принимает параметр `size` и инициализирует член `size` класса значением переданного аргумента. Модификатор `explicit` предотвращает неявное преобразование типов при вызове конструктора.

#### Метод получения элемента

```cpp
T get(size_t index) const
{
    if (data.count(index))
        return data.at(index);
    return 0;
}
```
Метод `get` возвращает значение элемента по индексу `index`. Если элемент существует в карте (`data`), возвращается его значение. Иначе возвращается ноль.

#### Метод установки элемента

```cpp
void set(size_t index, T value)
{
    if (index >= size)
        throw std::out_of_range("Index out of range");
    if (value != 0)
        data[index] = value;
    else
        data.erase(index);
}
```
Метод `set` устанавливает значение элемента по индексу `index`. Если индекс выходит за пределы допустимых значений, выбрасывается исключение `std::out_of_range`. Если значение равно нулю, соответствующий элемент удаляется из карты, иначе обновляется.

#### Метод получения размера вектора

```cpp
size_t getSize() const { return size; }
```
Метод `getSize` возвращает размер вектора.

#### Перегрузка оператора сложения и вычитания

```cpp
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
```
Перегруженный оператор `+/-` складывает/вычитает текущий объект с другим объектом типа `SparseVector`. Если размеры векторов различаются, выбрасывается исключение `std::invalid_argument`. Затем создается новый объект `result`, куда копируются все элементы текущего вектора и другого вектора, суммируя их значения.

#### Перегрузка оператора умножения на скаляр

```cpp
SparseVector<T> operator*(T scalar) const
{
    SparseVector<T> result(size);
    for (const auto &[index, value] : data)
        result.set(index, value * scalar);
    return result;
}
```
Оператор `*` умножает каждый элемент вектора на заданный скаляр `scalar`. Результат сохраняется в новом объекте `result`, который затем возвращается.

#### Метод вычисления скалярного произведения

```cpp
T dot(const SparseVector<T> &other) const
{
    if (size != other.size)
        throw std::invalid_argument("Vector sizes do not match");
    T result = 0;
    for (const auto &[index, value] : data)
        result += value * other.get(index);
    return result;
}
```
Метод `dot` вычисляет скалярное произведение двух векторов. Если размеры векторов различны, выбрасывается исключение `std::invalid_argument`. Скалярное произведение вычисляется путем суммирования произведений соответствующих элементов двух векторов.

#### Внутренний класс итератора

```cpp
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
```
Внутренний класс Iterator предоставляет интерфейс для итерации по элементам контейнера std::unordered_map<size_t, T>, используемого для хранения разреженных данных в классе SparseVector. Этот класс реализует стандартные операции для итераторов, такие как разыменование, инкрементация и сравнение, что делает возможным использование стандартных алгоритмов и циклов C++ для работы с элементами вектора.
Также создаются методы begin() и end() для итерации по элементам контейнера.


#### Вывод вектора
```cpp
    void print() const
    {
        for (size_t i = 0; i < size; ++i)
            std::cout << get(i) << " ";
        std::cout << "\n";
    }
```

# Создание класса и методов для работы с матрицами
## Шаблонный класс для разреженной матрицы

```cpp
template <typename T>
class SparseMatrix
{
```
Этот блок кода объявляет шаблонный класс `SparseMatrix`, который может работать с любым типом данных `T`.

---

```cpp
private:
    std::unordered_map<size_t, std::unordered_map<size_t, T>> data;
    size_t rows, cols;
```
Здесь определены приватные члены класса:
- `data`: карта, которая хранит данные разреженной матрицы. Внешняя карта использует строки как ключи, а внутренняя карта хранит столбцы и соответствующие им значения.
- `rows`: количество строк в матрице.
- `cols`: количество столбцов в матрице.

---

```cpp
public:
    SparseMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {}
```
Публичный конструктор класса `SparseMatrix`, принимающий параметры `rows` и `cols` для задания размеров матрицы. Эти параметры используются для инициализации членов `rows` и `cols`.

---

```cpp
T get(size_t row, size_t col) const
{
    if (data.count(row) && data.at(row).count(col))
        return data.at(row).at(col);
    return 0;
}
```
Метод `get` возвращает значение элемента матрицы по указанным координатам `row` и `col`. Если элемент отсутствует в карте, возвращается 0.

---

```cpp
void set(size_t row, size_t col, T value)
{
    if (row >= rows || col >= cols)
        throw std::out_of_range("Index out of range");
    if (value != 0)
        data[row][col] = value;
    else if (data.count(row))
        data[row].erase(col);
}
```
Метод `set` устанавливает значение элемента матрицы по координатам `row` и `col`. Если индексы выходят за границы матрицы, выбрасывается исключение `std::out_of_range`. Если значение равно 0, соответствующий элемент удаляется из внутренней карты.

---

```cpp
SparseMatrix<T> transpose() const
{
    SparseMatrix<T> result(cols, rows);
    for (const auto &[row, cols] : data)
        for (const auto &[col, value] : cols)
            result.set(col, row, value);
    return result;
}
```
Метод `transpose` возвращает транспонированную матрицу. Новая матрица `result` имеет размеры, обратные исходной (количество строк становится количеством столбцов и наоборот). Элементы исходной матрицы переписываются в транспонированную с измененными местами строк и столбцов.

---

```cpp
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
```
Перегруженный оператор `+` выполняет сложение двух матриц. Если размеры матриц не совпадают, выбрасывается исключение `std::invalid_argument`. Сложение происходит поэлементно, причем если какой-то элемент присутствует только в одной из матриц, он добавляется в результирующую матрицу.

---

```cpp
SparseMatrix<T> operator*(T scalar) const
{
    SparseMatrix<T> result(rows, cols);
    for (const auto &[row, cols] : data)
        for (const auto &[col, value] : cols)
            result.set(row, col, value * scalar);
    return result;
}
```
Перегруженный оператор `*` выполняет умножение всей матрицы на скаляр. Каждый элемент исходной матрицы умножается на данный скаляр, и результат записывается в новую матрицу `result`.

---
```cpp
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
```

Этот метод перегружает оператор * для выполнения умножения двух матриц. Если количество столбцов первой матрицы не соответствует количеству строк второй, то генерируется исключение std::invalid_argument. Матрица результата имеет размеры, равные числу строк первой матрицы и числу столбцов второй. Умножение выполняется по правилам линейной алгебры, где каждый элемент результирующей матрицы является суммой произведений элементов соответствующей строки первой матрицы и соответствующего столбца второй матрицы.

---
```cpp
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
```

Этот метод перегружает оператор * для выполнения умножения матрицы на вектор. Если количество столбцов матрицы не соответствует размеру вектора, то генерируется исключение std::invalid_argument. Результирующим вектором является сумма произведений элементов каждой строки матрицы на соответствующий элемент вектора.

---
```cpp
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
```

Этот метод вычисляет обратную матрицу для данной квадратной матрицы. Если матрица не является квадратной, то генерируется исключение std::invalid_argument. Реализован простой случай для матриц размером $2 \times 2$, но для большего размера матриц реализация отсутствует, и генерируется соответствующее исключение.

---
```cpp
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
```

Этот метод вычисляет степень матрицы. Если матрица не является квадратной или показатель степени отрицательный, то генерируется соответствующее исключение. Алгоритм основан на быстром возведении в степеньЭтот метод возводит матрицу в степень exponent. Если матрица не является квадратной или показатель степени отрицательный, то генерируется соответствующее исключение. Алгоритм основан на быстром возведении в степень методом бинарного возведения, что позволяет значительно ускорить вычисления для больших показателей.

---
```cpp
void print() const
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
            std::cout << get(i, j) << " ";
        std::cout << "\n";
    }
}
```
Этот метод выводит матрицу на экран построчно, разделяя элементы пробелом. Каждая строка завершается символом новой строки.
