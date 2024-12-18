#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

class MyClass
{
public:
    int value;

    MyClass(int val = 0) : value(val) {}

    bool operator<(const MyClass &other) const
    {
        return value < other.value;
    }

    bool operator==(const MyClass &other) const
    {
        return value == other.value;
    }
};

// Функция для вывода размеров списка/вектора
template <typename Container>
void printSize(const Container &container, const std::string &name)
{
    std::cout << name << " size: " << container.size() << std::endl;
}

// Функция для вывода элементов списка/вектора
template <typename Container>
void printElements(const Container &container, const std::string &name)
{
    std::cout << name << " elements: ";
    for (const auto &elem : container)
    {
        std::cout << elem.value << " ";
    }
    std::cout << std::endl;
}

// Специализация для списка пар (вывод пар элементов)
template <typename T>
void printElements(const std::list<std::pair<T, T>> &container, const std::string &name)
{
    std::cout << name << " pairs: ";
    for (const auto &pair : container)
    {
        std::cout << "(" << pair.first.value << ", " << pair.second.value << ") ";
    }
    std::cout << std::endl;
}

int main()
{
    // Генерация случайных чисел
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    std::uniform_int_distribution<int> size_dist(500, 1000);

    // Пункт 1: Создание вектора v1
    int size_v1 = size_dist(gen);
    std::vector<MyClass> v1(size_v1);
    for (auto &elem : v1)
        elem.value = dist(gen);

    // Пункт 2: Создание вектора v2 (последние 200 элементов или диапазон [b, e))
    int b = size_v1 - 200;
    b = std::max(0, b); // Убедимся, что b >= 0
    int e = size_v1;    // Конец диапазона
    std::vector<MyClass> v21(v1.begin() + b, v1.begin() + e);
    std::cout << "Sizes vectors, point 2 last 200: v1 = " << v1.size() << ", v2 = " << v21.size() << "\n";
    // Генерация произвольных b и e
    std::uniform_int_distribution<int> range_dist(0, size_v1 - 1);
    b = range_dist(gen);
    e = range_dist(gen);
    if (b > e)
        std::swap(b, e); // Гарантируем, что b <= e
    e = std::min(e, size_v1); // Убедимся, что e <= size_v1

    std::vector<MyClass> v2(v1.begin() + b, v1.begin() + e);
    std::cout << "Sizes vectors, point 2 with random b and e: v1 = " << v1.size() << ", v2 = " << v2.size() << "\n";

    // Пункт 3: Формируем список list1 из первых n наибольших элементов v1
    std::uniform_int_distribution<> dist2(20, 50);
    std::random_device rd; // seed for PRNG
    std::mt19937 mt_eng(rd());
    int n = dist2(mt_eng); // Число элементов от 20 до 50
    std::vector<MyClass> temp_v1(v1);
    std::partial_sort(temp_v1.begin(), temp_v1.begin() + n, temp_v1.end(),
                      [](const MyClass &a, const MyClass &b)
                      { return a.value > b.value; });
    std::list<MyClass> list1(temp_v1.begin(), temp_v1.begin() + n);

    // Пункт 4: Формируем список list2 из последних n наименьших элементов v2
    n = dist2(mt_eng); // Число элементов от 20 до 50
    std::nth_element(v2.begin(), v2.begin() + n, v2.end(),
                     [](const MyClass &a, const MyClass &b)
                     { return a.value < b.value; });
    std::list<MyClass> list2(v2.begin(), v2.begin() + n);

    std::cout << "Sizes lists points 3-4: list1 = " << list1.size() << ", list2 = " << list2.size() << "\n";


    // Удаление перемещённых элементов
    v1.erase(v1.begin() + b, v1.end());
    v2.erase(v2.begin(), v2.begin() + n);

    // Пункт 6: Средний элемент list1 и перегруппировка
    auto median_it = std::next(list1.begin(), list1.size() / 2);
    MyClass median_value = *median_it;
    list1.sort([median_value](const MyClass &a, const MyClass &b)
               { return a.value > median_value.value; });

    std::cout << "Median value of list1: " << median_value.value <<  "\n";
    
    // Пункт 7: Удаление нечётных элементов из list2
    list2.remove_if([](const MyClass &elem) { return elem.value % 2 != 0; });

    // Пункт 8: Вектор v3 из общих элементов v1 и v2
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    std::vector<MyClass> v3;
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v3));


    // Пункт 10: Формирование пар для list1 и list2 без приведения размеров
    std::list<std::pair<MyClass, MyClass>> list3_10;
    auto it1_10 = list1.begin();
    auto it2_10 = list2.begin();
    while (it1_10 != list1.end() && it2_10 != list2.end())
    {
        list3_10.emplace_back(*it1_10, *it2_10);
        ++it1_10;
        ++it2_10;
    }

    // Вывод размеров и элементов
    printSize(v1, "v1");
    printSize(v2, "v2");
    printSize(list1, "list1");
    printSize(list2, "list2");
    printSize(list3_10, "list3_10");
    printElements(list1, "list1");
    printElements(list2, "list2");
    printElements(list3_10, "list3");

    // Пункт 9: Приведение list1 и list2 к одинаковому размеру, формирование list3
    size_t min_size = std::min(list1.size(), list2.size());
    list1.resize(min_size);
    list2.resize(min_size);
    std::list<std::pair<MyClass, MyClass>> list3;
    auto it1 = list1.begin();
    auto it2 = list2.begin();
    while (it1 != list1.end() && it2 != list2.end())
    {
        list3.emplace_back(*it1, *it2);
        ++it1;
        ++it2;
    }

    // Вывод размеров и элементов
    printSize(v1, "v1");
    printSize(v2, "v2");
    printSize(list1, "list1");
    printSize(list2, "list2");
    printSize(list3, "list3_10");
    printElements(list1, "list1");
    printElements(list2, "list2");
    printElements(list3, "list3");

    return 0;
}
