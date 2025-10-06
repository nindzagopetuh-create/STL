//SequenceContainers
#include<iostream>
#include<array>
#include<vector>
#include<deque>
#include<list>
#include<forward_list>
#include<iterator>
using std::cin;
using std::cout;
using std::endl;
#define tab "\t"
#define delimiter "\n-------------------------------------\n"

template<typename T>void vector_info(const std::vector<T>& vec);
template<typename T>void deque_info(const std::deque<T>& vec);

//#define STL_ARRAY
//#define STL_VECTOR
//#define STL_DEQUE
//#define STL_LIST
#define STL_FORWARD_LIST

// Функция для вставки элемента по индексу в forward_list
template<typename T>
void insert_at_index(std::forward_list<T>& list, size_t index, const T& value)
{
    if (index == 0) {
        list.push_front(value);
        return;
    }

    auto it = list.before_begin();
    for (size_t i = 0; i < index; ++i) {
        if (std::next(it) == list.end()) {
            break; // Достигнут конец списка
        }
        ++it;
    }
    list.insert_after(it, value);
}

// Функция для удаления элемента по индексу из forward_list
template<typename T>
void erase_at_index(std::forward_list<T>& list, size_t index)
{
    if (list.empty()) return;

    if (index == 0) {
        list.pop_front();
        return;
    }

    auto prev = list.before_begin();
    auto curr = list.begin();

    for (size_t i = 0; i < index; ++i) {
        if (curr == list.end()) {
            return; // Индекс за пределами списка
        }
        ++prev;
        ++curr;
    }

    if (curr != list.end()) {
        list.erase_after(prev);
    }
}

// Функция для получения элемента по индексу (только для чтения)
template<typename T>
typename std::forward_list<T>::const_iterator get_at_index(const std::forward_list<T>& list, size_t index)
{
    auto it = list.cbegin();
    for (size_t i = 0; i < index && it != list.cend(); ++i) {
        ++it;
    }
    return it;
}

// Функция для печати forward_list
template<typename T>
void print_forward_list(const std::forward_list<T>& list)
{
    for (const auto& element : list) {
        cout << element << tab;
    }
    cout << endl;
}

void main()
{
    setlocale(LC_ALL, "");

#ifdef STL_FORWARD_LIST
    std::forward_list<int> list = { 3, 5, 8, 13, 21 };
    list.push_front(2);
    list.push_front(1);
    list.push_front(1);
    list.push_front(0);

    cout << "Исходный список: ";
    print_forward_list(list);

    // Добавление элемента по индексу
    int index;
    int value;

    // Добавление элемента
    cout << "Введите индекс добавляемого элемента: "; cin >> index;
    cout << "Введите значение добавляемого элемента: "; cin >> value;

    insert_at_index(list, index, value);
    cout << "Список после добавления: ";
    print_forward_list(list);

    // Удаление элемента по индексу
    cout << "Введите индекс удаляемого элемента: "; cin >> index;

    erase_at_index(list, index);
    cout << "Список после удаления: ";
    print_forward_list(list);

    // Демонстрация нескольких операций
    cout << delimiter << "Дополнительные операции:" << endl;

    // Добавляем несколько элементов
    insert_at_index(list, 2, 100);
    insert_at_index(list, 0, -1); // В начало
    insert_at_index(list, 10, 999); // В конец (если индекс больше размера)

    cout << "Список после нескольких добавлений: ";
    print_forward_list(list);

    // Удаляем несколько элементов
    erase_at_index(list, 0); // Первый элемент
    erase_at_index(list, 3); // Элемент в середине

    cout << "Список после нескольких удалений: ";
    print_forward_list(list);

#endif // STL_FORWARD_LIST

}

template<typename T>void vector_info(const std::vector<T>& vec)
{
    cout << "Size:\t " << vec.size() << endl;
    cout << "MaxSize: " << vec.max_size() << endl;
    cout << "Capacity:" << vec.capacity() << endl;
    cout << delimiter << endl;
}

template<typename T>void deque_info(const std::deque<T>& vec)
{
    cout << "Size:\t " << vec.size() << endl;
    cout << "MaxSize: " << vec.max_size() << endl;
    cout << delimiter << endl;
}