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

    // Оператор сравнения для сортировки
    bool operator<(const MyClass &other) const
    {
        return value < other.value;
    }

    bool operator==(const MyClass &other) const
    {
        return value == other.value;
    }
};

int main()
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);

    // Создание вектора v1 (500-1000 элементов) со случайными значениями
    std::uniform_int_distribution<int> size_dist(500, 1000);
    int size_v1 = size_dist(gen);
    std::vector<MyClass> v1(size_v1);
    for (auto &elem : v1)
    {
        elem.value = dist(gen);
    }

    // Пункт 2: Создаём вектор v2 из последних 200 элементов вектора v1
    int b = size_v1 - 200; // Начальная позиция копирования
    if (b < 0)
        b = 0;       // Если вектор меньше 200 элементов, начинаем с начала
    int e = size_v1; // Конечная позиция копирования
    std::vector<MyClass> v21(v1.begin() + b, v1.begin() + e);
    std::cout << "Sizes vectors, point 2 last 200: v1 = " << v1.size() << ", v2 = " << v21.size() << "\n";

    // Рассматриваем произвольные b и e
    std::uniform_int_distribution<int> range_dist(0, size_v1 - 1);
    b = range_dist(gen); // Случайная начальная позиция
    e = range_dist(gen); // Случайная конечная позиция
    if (b > e)
        std::swap(b, e); // Если b больше e, меняем их местами
    if (e > size_v1)
        e = size_v1; // Убеждаемся, что e не выходит за пределы
    // Создаём вектор v2 на основе диапазона [b, e)
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

    // Удаляем перемещённые элементы из v1 и v2
    v1.erase(v1.begin() + b, v1.end());
    v2.erase(v2.begin(), v2.begin() + n);

    // Пункт 6: Находим средний элемент в list1 и перегруппируем элементы
    auto median_it = std::next(list1.begin(), list1.size() / 2);
    MyClass median_value = *median_it;
    list1.sort([median_value](const MyClass &a, const MyClass &b)
               { return a.value > median_value.value; });
    std::cout << "Median value of list1: " << median_value.value << std::endl;

    // Пункт 7: Удаляем нечётные элементы из list2
    list2.remove_if([](const MyClass &elem)
                    { return elem.value % 2 != 0; });

    // Пункт 8: Создаем вектор v3 из общих элементов v1 и v2
    std::sort(v1.begin(), v1.end(), [](const MyClass &a, const MyClass &b)
              { return a.value < b.value; });
    std::sort(v2.begin(), v2.end(), [](const MyClass &a, const MyClass &b)
              { return a.value < b.value; });
    std::vector<MyClass> v3;
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                          std::back_inserter(v3));

    

    // Пункт 10: Формирование пар для v1 и v2 без приведения размеров
    std::list<std::pair<MyClass, MyClass>> list3_10;
    auto it1_10 = list1.begin();
    auto it2_10 = list2.begin();
    while (it1_10 != list1.end() && it2_10 != list2.end())
    {
        list3_10.emplace_back(*it1_10, *it2_10);
        ++it1_10;
        ++it2_10;
    }

    std::cout << "Sizes vectors point 10: v1 = " << v1.size() << ", v2 = " << v2.size()
              << ", v3 = " << v3.size() << "\n";

    std::cout << "Sizes lists point 10: list1 = " << list1.size() << ", list2 = " << list2.size()
              << ", list3 = " << list3_10.size() << "\n";

    for (const auto &pair : list1)
    {
        std::cout << pair.value << " ";
    }
    std::cout << std::endl;
    for (const auto &pair : list2)
    {
        std::cout << pair.value << " ";
    }
    std::cout << std::endl;
    for (const auto &pair : list3_10)
    {
        std::cout << "(" << pair.first.value << ", " << pair.second.value << ") ";
    }
    std::cout << std::endl;
    // Пункт 9: Приводим списки list1 и list2 к одинаковому размеру и формируем list3
    size_t min_size = std::min(list1.size(), list2.size());
    while (list1.size() > min_size)
        list1.pop_back();
    while (list2.size() > min_size)
        list2.pop_back();
    std::list<std::pair<MyClass, MyClass>> list3;
    auto it1 = list1.begin();
    auto it2 = list2.begin();
    while (it1 != list1.end() && it2 != list2.end())
    {
        list3.emplace_back(*it1, *it2);
        ++it1;
        ++it2;
    }

    // Вывод для проверки
    std::cout << "Sizes vectors point 9: v1 = " << v1.size() << ", v2 = " << v2.size()
              << ", v3 = " << v3.size() << "\n";

    std::cout << "Sizes lists point 9: list1 = " << list1.size() << ", list2 = " << list2.size()
              << ", list3 = " << list3.size() << "\n";

    for (const auto &pair : list1)
    {
        std::cout << pair.value << " ";
    }
    std::cout << std::endl;
    for (const auto &pair : list2)
    {
        std::cout << pair.value << " ";
    }
    std::cout << std::endl;
    for (const auto &pair : list3)
    {
        std::cout << "(" << pair.first.value << ", " << pair.second.value << ") ";
    }

    return 0;
}
