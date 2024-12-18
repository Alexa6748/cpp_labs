#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <array>
#include <iostream>
#include <string>

// Массив строковых представлений чисел от 0 до 19
static const std::array<std::string, 20> units = {
    "null", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine",
    "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
// Массив строковых представлений десятков
static const std::array<std::string, 10> tens = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", 
    "eighty", "ninety"};

class Number
{
public:
    // Конструктор по умолчанию
    Number();

    // Конструктор с параметрами
    Number(int value);

    // Конструктор копирования
    Number(const Number &other);

    // Конструктор перемещения
    Number(Number &&other) noexcept;

    // Деструктор
    ~Number();

    // Операция присваивания (без перемещения)
    Number &operator=(const Number &other);

    // Операция присваивания (с перемещением)
    Number &operator=(Number &&other) noexcept;

    // Метод для получения числового значения
    int getValue() const { return m_value; }

    // Метод для получения строкового представления

    std::string getStringRepresentation() const
    {
        if (m_value == 0)
        {
            return "null"; // Специальная обработка нуля
        }

        std::string m_string_representation;

        int hundreds = m_value / 100;
        int remainder = m_value % 100;

        if (hundreds > 0)
        {
            m_string_representation += units[hundreds] + " hundred ";
        }

        if (remainder >= 20)
        {
            int tens_digit = remainder / 10;
            int ones_digit = remainder % 10;

            m_string_representation += tens[tens_digit];

            if (ones_digit > 0)
            {
                m_string_representation += "-" + units[ones_digit];
            }
        }
        else if (remainder > 0 && remainder <= 19)
        {
            m_string_representation += units[remainder];
        }
        
        return m_string_representation;
    }

private:
    int m_value;
    std::string m_string_representation;
};

#endif // NUMBER_HPP