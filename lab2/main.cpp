#include "number.hpp"
#include <iostream>
#include <vector>
#include <list>

void printVector(const std::vector<Number>& vec)
{
    int i = 0;
    std::cout << "Printed vector" << std::endl;
    for (auto& num : vec) {
        std::cout << "Value " + std::to_string(i++) + " : " << num.getValue() << ", String representation: " << num.getStringRepresentation() << std::endl;
    }
}

void printList(const std::list<Number>& lst)
{
    int i = 0;
    std::cout << "Printed List" << std::endl;
    for (auto& num : lst) {
        std::cout << "Value " + std::to_string(i++) + " : " << num.getValue() << ", String representation: " << num.getStringRepresentation() << std::endl;
    }
}

Number createNumber(int value)
{
    return Number(value); // Создаем объект и возвращаем его
}

int main()
{
    std::cout << "--- Creating static instances ---" << std::endl;
    Number n1(42);       // Статический экземпляр
    Number n2(n1);       // Копия n1
    Number n3(std::move(n2)); // Перемещение n2

    std::cout << "\n--- Creating dynamic instances ---" << std::endl;
    Number* p_n4 = new Number(100);   // Динамический экземпляр
    delete p_n4;                      // Удаление динамического экземпляра

    std::cout << "\n--- Passing to function by value ---" << std::endl;
    Number n5 = createNumber(55);     // Передача и возврат по значению

    std::cout << "\n--- Passing to function by reference ---" << std::endl;
    Number n6(66);
    Number& ref_n6 = n6;              // Ссылка на n6
    Number n7(ref_n6);                // Копируем через ссылку

    std::cout << "\n--- Working with vectors and lists ---" << std::endl;
    std::vector<Number> numbers_vec;
    numbers_vec.push_back(Number(11));
    numbers_vec.push_back(Number(22));
    numbers_vec.push_back(Number(33));
    numbers_vec.push_back(Number(44));
    numbers_vec.push_back(Number(55));
    numbers_vec.push_back(Number(66));
    printVector(numbers_vec);

    std::list<Number> numbers_list;
    numbers_list.push_front(Number(4));
    numbers_list.push_front(Number(5));
    numbers_list.push_front(Number(6));
    numbers_list.push_front(Number(3));
    numbers_list.push_front(Number(2));
    numbers_list.push_front(Number(1));
    printList(numbers_list);

    return 0;
}
