#include <iostream>
#include <stdexcept>
#include <limits>

class FactorialException : public std::exception {
private:
    std::string message;

public:
    FactorialException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

unsigned long long Factorial(int n) {
    if (n < 0) {
        throw FactorialException("Factorial is not defined for negative numbers");
    }

    if (n == 0 || n == 1) {
        return 1;
    }

    unsigned long long result = 1;
    unsigned long long previous = 1;

    for (int i = 2; i <= n; ++i) {
        // Проверка на переполнение перед умножением
        if (result > std::numeric_limits<unsigned long long>::max() / i) {
            throw FactorialException("Arithmetic overflow in factorial calculation");
        }

        previous = result;
        result *= i;

        // Дополнительная проверка после умножения
        if (result < previous) {
            throw FactorialException("Arithmetic overflow in factorial calculation");
        }
    }

    return result;
}

// Альтернативная версия с использованием встроенного исключения
unsigned long long FactorialWithStdException(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }

    unsigned long long result = 1;

    for (int i = 2; i <= n; ++i) {
        if (result > std::numeric_limits<unsigned long long>::max() / i) {
            throw std::overflow_error("Arithmetic overflow in factorial calculation");
        }
        result *= i;
    }

    return result;
}
#include <iostream>

void testFactorial(int n) {
    try {
        unsigned long long result = Factorial(n);
        std::cout << n << "! = " << result << std::endl;
    }
    catch (const FactorialException& e) {
        std::cout << "Error for " << n << "!: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Standard exception for " << n << "!: " << e.what() << std::endl;
    }
}

int main() {
    // Тестирование функции
    testFactorial(5);    // 120
    testFactorial(10);   // 3628800
    testFactorial(20);   // 2432902008176640000
    testFactorial(30);   // Переполнение
    testFactorial(-5);   // Ошибка: отрицательное число

    return 0;
}