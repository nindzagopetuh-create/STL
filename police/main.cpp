#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// Структура для хранения информации о правонарушении
struct Violation {
    std::string date;
    std::string description;
    double fine_amount;

    Violation(const std::string& d, const std::string& desc, double fine)
        : date(d), description(desc), fine_amount(fine) {
    }
};

// Узел бинарного дерева
struct TreeNode {
    std::string car_number;
    std::vector<Violation> violations;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const std::string& number)
        : car_number(number), left(nullptr), right(nullptr) {
    }
};

// Класс для базы данных ГАИ
class PoliceDatabase {
private:
    TreeNode* root;

    // Рекурсивные вспомогательные методы
    TreeNode* insert(TreeNode* node, const std::string& car_number, const Violation& violation) {
        if (node == nullptr) {
            TreeNode* newNode = new TreeNode(car_number);
            newNode->violations.push_back(violation);
            return newNode;
        }

        if (car_number < node->car_number) {
            node->left = insert(node->left, car_number, violation);
        }
        else if (car_number > node->car_number) {
            node->right = insert(node->right, car_number, violation);
        }
        else {
            // Номер уже существует, добавляем нарушение в список
            node->violations.push_back(violation);
        }
        return node;
    }

    TreeNode* find(TreeNode* node, const std::string& car_number) const {
        if (node == nullptr || node->car_number == car_number) {
            return node;
        }

        if (car_number < node->car_number) {
            return find(node->left, car_number);
        }
        else {
            return find(node->right, car_number);
        }
    }

    void printInOrder(TreeNode* node) const {
        if (node != nullptr) {
            printInOrder(node->left);
            printCarData(node);
            printInOrder(node->right);
        }
    }

    void printInRange(TreeNode* node, const std::string& start, const std::string& end) const {
        if (node != nullptr) {
            if (node->car_number >= start) {
                printInRange(node->left, start, end);
            }

            if (node->car_number >= start && node->car_number <= end) {
                printCarData(node);
            }

            if (node->car_number <= end) {
                printInRange(node->right, start, end);
            }
        }
    }

    void saveToFile(TreeNode* node, std::ofstream& file) const {
        if (node != nullptr) {
            saveToFile(node->left, file);

            // Сохраняем данные об автомобиле и его нарушениях
            for (const auto& violation : node->violations) {
                file << node->car_number << "|"
                    << violation.date << "|"
                    << violation.description << "|"
                    << violation.fine_amount << "\n";
            }

            saveToFile(node->right, file);
        }
    }

    void clearTree(TreeNode* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    void printCarData(TreeNode* node) const {
        std::cout << "Автомобиль: " << node->car_number << "\n";
        std::cout << "Количество нарушений: " << node->violations.size() << "\n";

        double total_fine = 0;
        for (size_t i = 0; i < node->violations.size(); ++i) {
            std::cout << "  " << i + 1 << ". Дата: " << node->violations[i].date
                << ", Описание: " << node->violations[i].description
                << ", Штраф: " << node->violations[i].fine_amount << " руб.\n";
            total_fine += node->violations[i].fine_amount;
        }
        std::cout << "  Общая сумма штрафов: " << total_fine << " руб.\n";
        std::cout << "----------------------------------------\n";
    }

public:
    PoliceDatabase() : root(nullptr) {}

    ~PoliceDatabase() {
        clearTree(root);
    }

    // Добавление квитанции
    void addTicket(const std::string& car_number, const std::string& date,
        const std::string& description, double fine_amount) {
        Violation violation(date, description, fine_amount);
        root = insert(root, car_number, violation);
        std::cout << "Квитанция добавлена для автомобиля " << car_number << "\n";
    }

    // Полная распечатка базы данных
    void printAll() const {
        std::cout << "=== ПОЛНАЯ БАЗА ДАННЫХ ГАИ ===\n";
        if (root == nullptr) {
            std::cout << "База данных пуста.\n";
        }
        else {
            printInOrder(root);
        }
        std::cout << "==============================\n";
    }

    // Распечатка данных по заданному номеру
    void printByNumber(const std::string& car_number) const {
        TreeNode* node = find(root, car_number);
        if (node != nullptr) {
            std::cout << "=== ДАННЫЕ ПО АВТОМОБИЛЮ " << car_number << " ===\n";
            printCarData(node);
        }
        else {
            std::cout << "Автомобиль с номером " << car_number << " не найден.\n";
        }
    }

    // Распечатка данных по диапазону номеров
    void printByRange(const std::string& start_number, const std::string& end_number) const {
        std::cout << "=== ДАННЫЕ ПО ДИАПАЗОНУ НОМЕРОВ " << start_number
            << " - " << end_number << " ===\n";
        printInRange(root, start_number, end_number);
        std::cout << "===================================\n";
    }

    // Сохранение в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка открытия файла для записи: " << filename << "\n";
            return;
        }

        saveToFile(root, file);
        file.close();
        std::cout << "Данные сохранены в файл: " << filename << "\n";
    }

    // Загрузка из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка открытия файла для чтения: " << filename << "\n";
            return;
        }

        // Очищаем текущее дерево
        clearTree(root);
        root = nullptr;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);

            if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
                std::string car_number = line.substr(0, pos1);
                std::string date = line.substr(pos1 + 1, pos2 - pos1 - 1);
                std::string description = line.substr(pos2 + 1, pos3 - pos2 - 1);
                double fine_amount = std::stod(line.substr(pos3 + 1));

                addTicket(car_number, date, description, fine_amount);
            }
        }

        file.close();
        std::cout << "Данные загружены из файла: " << filename << "\n";
    }

    // Проверка пустоты базы данных
    bool isEmpty() const {
        return root == nullptr;
    }
};

// Функция для отображения меню
void displayMenu() {
    std::cout << "\n=== СИСТЕМА БАЗЫ ДАННЫХ ГАИ ===\n";
    std::cout << "1. Добавить квитанцию\n";
    std::cout << "2. Полная распечатка базы данных\n";
    std::cout << "3. Поиск по номеру автомобиля\n";
    std::cout << "4. Поиск по диапазону номеров\n";
    std::cout << "5. Сохранить в файл\n";
    std::cout << "6. Загрузить из файла\n";
    std::cout << "7. Выход\n";
    std::cout << "Выберите действие: ";
}

int main() {
    PoliceDatabase database;
    int choice;

    // Установка локализации для русского языка
    setlocale(LC_ALL, "Russian");

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // Очистка буфера

        switch (choice) {
        case 1: {
            std::string car_number, date, description;
            double fine;

            std::cout << "Введите номер автомобиля: ";
            std::getline(std::cin, car_number);
            std::cout << "Введите дату нарушения: ";
            std::getline(std::cin, date);
            std::cout << "Введите описание нарушения: ";
            std::getline(std::cin, description);
            std::cout << "Введите сумму штрафа: ";
            std::cin >> fine;
            std::cin.ignore();

            database.addTicket(car_number, date, description, fine);
            break;
        }

        case 2:
            database.printAll();
            break;

        case 3: {
            std::string car_number;
            std::cout << "Введите номер автомобиля для поиска: ";
            std::getline(std::cin, car_number);
            database.printByNumber(car_number);
            break;
        }

        case 4: {
            std::string start, end;
            std::cout << "Введите начальный номер диапазона: ";
            std::getline(std::cin, start);
            std::cout << "Введите конечный номер диапазона: ";
            std::getline(std::cin, end);
            database.printByRange(start, end);
            break;
        }

        case 5: {
            std::string filename;
            std::cout << "Введите имя файла для сохранения: ";
            std::getline(std::cin, filename);
            database.saveToFile(filename);
            break;
        }

        case 6: {
            std::string filename;
            std::cout << "Введите имя файла для загрузки: ";
            std::getline(std::cin, filename);
            database.loadFromFile(filename);
            break;
        }

        case 7:
            std::cout << "Выход из программы.\n";
            break;

        default:
            std::cout << "Неверный выбор. Попробуйте снова.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}