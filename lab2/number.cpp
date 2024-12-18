#include "number.hpp"
#include <sstream>

// Конструктор по умолчанию
Number::Number()
{
    std::cout << "Default constructor" << std::endl;
    m_value = 0;
    m_string_representation = "ноль";
}

// Конструктор с параметрами
Number::Number(int value)
{
    std::cout << "Parameterized constructor" << std::endl;
    m_value = value;
    std::stringstream ss;
    ss << "Integer value is: " << value;
    m_string_representation = ss.str();
}

// Конструктор копирования
Number::Number(const Number& other)
{
    std::cout << "Copy constructor" << std::endl;
    m_value = other.m_value;
    m_string_representation = other.m_string_representation;
}

// Конструктор перемещения
Number::Number(Number&& other) noexcept
{
    std::cout << "Move constructor" << std::endl;
    m_value = other.m_value;
    m_string_representation = std::move(other.m_string_representation);
    other.m_value = 0;
    other.m_string_representation.clear(); // Очищаем строку у исходного объекта
}

// Деструктор
Number::~Number()
{
    std::cout << "Destructor" << std::endl;
}

// Операция присваивания (без перемещения)
Number& Number::operator=(const Number& other)
{
    if (this != &other) {
        std::cout << "Assignment operator (copy)" << std::endl;
        m_value = other.m_value;
        m_string_representation = other.m_string_representation;
    }
    return *this;
}

// Операция присваивания (с перемещением)
Number& Number::operator=(Number&& other) noexcept
{
    if (this != &other) {
        std::cout << "Assignment operator (move)" << std::endl;
        m_value = other.m_value;
        m_string_representation = std::move(other.m_string_representation);
        other.m_value = 0;
        other.m_string_representation.clear(); // Очищаем строку у исходного объекта
    }
    return *this;
}

