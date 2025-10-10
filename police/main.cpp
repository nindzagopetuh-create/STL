#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<list>
#include<ctime>
using std::cin;
using std::cout;
using std::endl;

#define delimiter "\n--------------------------------------------\n"

const std::map<int, std::string> VIOLATIONS =
{
	{0, "N/A"},
	{1, "Парковка в неположенном месте"},
	{2, "Непристегутый ремень безопасности"},
	{3, "Пересечение сплошной"},
	{4, "Превышение скорости"},
	{5, "Проезд на красный"},
	{6, "Выезд на встречную полосу"},
	{7, "Езда в нетрезвом состоянии"},
	{8, "Оскорбление офицера"},
};

class Crime;
std::stringstream& operator>>(std::stringstream& stream, Crime& obj);

class Crime
{
	int violation;
	std::string place;
	std::string timestamp;  // Время правонарушения

public:
	int get_violation()const { return violation; }
	const std::string& get_place()const { return place; }
	const std::string& get_timestamp()const { return timestamp; }

	void set_violation(int violation) { this->violation = violation; }
	void set_place(const std::string& place) { this->place = place; }
	void set_timestamp(const std::string& timestamp) { this->timestamp = timestamp; }

	// Получить текущее время в формате строки
	static std::string get_current_time() {
		std::time_t now = std::time(nullptr);
		std::tm* local_time = std::localtime(&now);
		char buffer[80];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
		return std::string(buffer);
	}

	Crime(int violation, const std::string& place) {
		set_violation(violation);
		set_place(place);
		set_timestamp(get_current_time());  // Устанавливаем текущее время
	}

	Crime(int violation, const std::string& place, const std::string& timestamp) {
		set_violation(violation);
		set_place(place);
		set_timestamp(timestamp);
	}

	explicit Crime(const std::string& str) {
		std::stringstream stream(str);
		stream >> *this;
	}
};

// Оператор вывода для консоли
std::ostream& operator<<(std::ostream& os, const Crime& obj) {
	os.width(44);
	os << std::left;
	os << VIOLATIONS.at(obj.get_violation());
	os << " Место: " << obj.get_place();
	os << " Время: " << obj.get_timestamp();
	return os;
}

// Оператор вывода в файл
std::ofstream& operator<<(std::ofstream& ofs, const Crime& obj) {
	ofs << obj.get_violation() << " " << obj.get_place() << " " << obj.get_timestamp();
	return ofs;
}

// Оператор ввода из потока
std::stringstream& operator>>(std::stringstream& stream, Crime& obj) {
	int violation;
	stream >> violation;

	std::string place;
	std::getline(stream, place);

	// Разделяем место и время (время идет после последнего пробела)
	size_t last_space = place.find_last_of(' ');
	if (last_space != std::string::npos) {
		std::string timestamp = place.substr(last_space + 1);
		place = place.substr(0, last_space);
		obj.set_timestamp(timestamp);
	}
	else {
		obj.set_timestamp(Crime::get_current_time());
	}

	obj.set_violation(violation);
	obj.set_place(place);
	return stream;
}

void print(const std::map<std::string, std::list<Crime>>& base);
void save(const std::map<std::string, std::list<Crime>>& base, const std::string& filename);
std::map<std::string, std::list<Crime>> load(const std::string& filename);

//#define INIT_BASE

int main() {
	setlocale(LC_ALL, "");

#ifdef INIT_BASE
	std::map<std::string, std::list<Crime>> base =
	{
		{"a123bb", {Crime(2, "ул. Пролетарская"), Crime(3, "ул. Ватутина")}},
		{"a777aa", {Crime(4, "ул. Ленина"), Crime(5, "ул. Ленина"), Crime(7, "ул. Энтузиастов"), Crime(8, "ул. Энтузиастов")}},
		{"a001eg", {Crime(5, "ул. Октябрьская"), Crime(5, "ул. Октябрьская"), Crime(7, "ул. Космическая"), Crime(8, "ул. Энтузиастов")}}
	};
	print(base);
	save(base, "base.txt");
#endif // INIT_BASE

	std::map<std::string, std::list<Crime>> base = load("base.txt");
	print(base);

	return 0;
}

void print(const std::map<std::string, std::list<Crime>>& base) {
	for (auto& plate : base) {
		cout << "Номер: " << plate.first << ":\n";
		for (auto& violation : plate.second) {
			cout << "\t" << violation << endl;
		}
		cout << delimiter << endl;
	}
}

void save(const std::map<std::string, std::list<Crime>>& base, const std::string& filename) {
	std::ofstream fout(filename);
	for (auto& plate : base) {
		fout << plate.first << ":";
		for (auto& violation : plate.second) {
			fout << violation << ",";  // Теперь сохраняется с временем
		}
		fout << endl;
	}
	fout.close();
	std::string cmd = "notepad ";
	cmd += filename;
	system(cmd.c_str());
}

std::map<std::string, std::list<Crime>> load(const std::string& filename) {
	std::map<std::string, std::list<Crime>> base;
	std::ifstream fin(filename);
	if (fin.is_open()) {
		while (!fin.eof()) {
			std::string licence_plate;
			std::getline(fin, licence_plate, ':');
			if (licence_plate.empty()) continue;

			cout << "Загрузка данных для: " << licence_plate << "\t";

			const int SIZE = 1024 * 500;
			char all_crimes[SIZE];
			fin.getline(all_crimes, SIZE);
			cout << "Нарушения: " << all_crimes << endl;

			const char delimiters[] = ",";
			for (char* pch = strtok(all_crimes, delimiters); pch; pch = strtok(NULL, delimiters)) {
				base[licence_plate].push_back(Crime(pch));
			}
		}
	}
	else {
		std::cerr << "Error: File not found" << endl;
	}
	fin.close();
	return base;
}