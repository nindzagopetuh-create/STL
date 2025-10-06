#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// ��������� ��� �������� ���������� � ��������������
struct Violation {
    std::string date;
    std::string description;
    double fine_amount;

    Violation(const std::string& d, const std::string& desc, double fine)
        : date(d), description(desc), fine_amount(fine) {
    }
};

// ���� ��������� ������
struct TreeNode {
    std::string car_number;
    std::vector<Violation> violations;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const std::string& number)
        : car_number(number), left(nullptr), right(nullptr) {
    }
};

// ����� ��� ���� ������ ���
class PoliceDatabase {
private:
    TreeNode* root;

    // ����������� ��������������� ������
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
            // ����� ��� ����������, ��������� ��������� � ������
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

            // ��������� ������ �� ���������� � ��� ����������
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
        std::cout << "����������: " << node->car_number << "\n";
        std::cout << "���������� ���������: " << node->violations.size() << "\n";

        double total_fine = 0;
        for (size_t i = 0; i < node->violations.size(); ++i) {
            std::cout << "  " << i + 1 << ". ����: " << node->violations[i].date
                << ", ��������: " << node->violations[i].description
                << ", �����: " << node->violations[i].fine_amount << " ���.\n";
            total_fine += node->violations[i].fine_amount;
        }
        std::cout << "  ����� ����� �������: " << total_fine << " ���.\n";
        std::cout << "----------------------------------------\n";
    }

public:
    PoliceDatabase() : root(nullptr) {}

    ~PoliceDatabase() {
        clearTree(root);
    }

    // ���������� ���������
    void addTicket(const std::string& car_number, const std::string& date,
        const std::string& description, double fine_amount) {
        Violation violation(date, description, fine_amount);
        root = insert(root, car_number, violation);
        std::cout << "��������� ��������� ��� ���������� " << car_number << "\n";
    }

    // ������ ���������� ���� ������
    void printAll() const {
        std::cout << "=== ������ ���� ������ ��� ===\n";
        if (root == nullptr) {
            std::cout << "���� ������ �����.\n";
        }
        else {
            printInOrder(root);
        }
        std::cout << "==============================\n";
    }

    // ���������� ������ �� ��������� ������
    void printByNumber(const std::string& car_number) const {
        TreeNode* node = find(root, car_number);
        if (node != nullptr) {
            std::cout << "=== ������ �� ���������� " << car_number << " ===\n";
            printCarData(node);
        }
        else {
            std::cout << "���������� � ������� " << car_number << " �� ������.\n";
        }
    }

    // ���������� ������ �� ��������� �������
    void printByRange(const std::string& start_number, const std::string& end_number) const {
        std::cout << "=== ������ �� ��������� ������� " << start_number
            << " - " << end_number << " ===\n";
        printInRange(root, start_number, end_number);
        std::cout << "===================================\n";
    }

    // ���������� � ����
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "������ �������� ����� ��� ������: " << filename << "\n";
            return;
        }

        saveToFile(root, file);
        file.close();
        std::cout << "������ ��������� � ����: " << filename << "\n";
    }

    // �������� �� �����
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "������ �������� ����� ��� ������: " << filename << "\n";
            return;
        }

        // ������� ������� ������
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
        std::cout << "������ ��������� �� �����: " << filename << "\n";
    }

    // �������� ������� ���� ������
    bool isEmpty() const {
        return root == nullptr;
    }
};

// ������� ��� ����������� ����
void displayMenu() {
    std::cout << "\n=== ������� ���� ������ ��� ===\n";
    std::cout << "1. �������� ���������\n";
    std::cout << "2. ������ ���������� ���� ������\n";
    std::cout << "3. ����� �� ������ ����������\n";
    std::cout << "4. ����� �� ��������� �������\n";
    std::cout << "5. ��������� � ����\n";
    std::cout << "6. ��������� �� �����\n";
    std::cout << "7. �����\n";
    std::cout << "�������� ��������: ";
}

int main() {
    PoliceDatabase database;
    int choice;

    // ��������� ����������� ��� �������� �����
    setlocale(LC_ALL, "Russian");

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(); // ������� ������

        switch (choice) {
        case 1: {
            std::string car_number, date, description;
            double fine;

            std::cout << "������� ����� ����������: ";
            std::getline(std::cin, car_number);
            std::cout << "������� ���� ���������: ";
            std::getline(std::cin, date);
            std::cout << "������� �������� ���������: ";
            std::getline(std::cin, description);
            std::cout << "������� ����� ������: ";
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
            std::cout << "������� ����� ���������� ��� ������: ";
            std::getline(std::cin, car_number);
            database.printByNumber(car_number);
            break;
        }

        case 4: {
            std::string start, end;
            std::cout << "������� ��������� ����� ���������: ";
            std::getline(std::cin, start);
            std::cout << "������� �������� ����� ���������: ";
            std::getline(std::cin, end);
            database.printByRange(start, end);
            break;
        }

        case 5: {
            std::string filename;
            std::cout << "������� ��� ����� ��� ����������: ";
            std::getline(std::cin, filename);
            database.saveToFile(filename);
            break;
        }

        case 6: {
            std::string filename;
            std::cout << "������� ��� ����� ��� ��������: ";
            std::getline(std::cin, filename);
            database.loadFromFile(filename);
            break;
        }

        case 7:
            std::cout << "����� �� ���������.\n";
            break;

        default:
            std::cout << "�������� �����. ���������� �����.\n";
            break;
        }
    } while (choice != 7);

    return 0;
}