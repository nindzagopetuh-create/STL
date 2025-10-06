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

// ������� ��� ������� �������� �� ������� � forward_list
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
            break; // ��������� ����� ������
        }
        ++it;
    }
    list.insert_after(it, value);
}

// ������� ��� �������� �������� �� ������� �� forward_list
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
            return; // ������ �� ��������� ������
        }
        ++prev;
        ++curr;
    }

    if (curr != list.end()) {
        list.erase_after(prev);
    }
}

// ������� ��� ��������� �������� �� ������� (������ ��� ������)
template<typename T>
typename std::forward_list<T>::const_iterator get_at_index(const std::forward_list<T>& list, size_t index)
{
    auto it = list.cbegin();
    for (size_t i = 0; i < index && it != list.cend(); ++i) {
        ++it;
    }
    return it;
}

// ������� ��� ������ forward_list
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

    cout << "�������� ������: ";
    print_forward_list(list);

    // ���������� �������� �� �������
    int index;
    int value;

    // ���������� ��������
    cout << "������� ������ ������������ ��������: "; cin >> index;
    cout << "������� �������� ������������ ��������: "; cin >> value;

    insert_at_index(list, index, value);
    cout << "������ ����� ����������: ";
    print_forward_list(list);

    // �������� �������� �� �������
    cout << "������� ������ ���������� ��������: "; cin >> index;

    erase_at_index(list, index);
    cout << "������ ����� ��������: ";
    print_forward_list(list);

    // ������������ ���������� ��������
    cout << delimiter << "�������������� ��������:" << endl;

    // ��������� ��������� ���������
    insert_at_index(list, 2, 100);
    insert_at_index(list, 0, -1); // � ������
    insert_at_index(list, 10, 999); // � ����� (���� ������ ������ �������)

    cout << "������ ����� ���������� ����������: ";
    print_forward_list(list);

    // ������� ��������� ���������
    erase_at_index(list, 0); // ������ �������
    erase_at_index(list, 3); // ������� � ��������

    cout << "������ ����� ���������� ��������: ";
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