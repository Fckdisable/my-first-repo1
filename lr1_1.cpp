#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>

using namespace std;

class Student {
public:
    string lastName;
    string firstName;
    string middleName;
    int birthYear;
    string address;
    string phone;
    string faculty;
    int course;

    int getAge(int currentYear) const {
        return currentYear - birthYear;
    }

    void show() const {
        cout << lastName << " " << firstName << " " << middleName
             << ", Год рождения: " << birthYear
             << ", Адрес: " << address
             << ", Телефон: " << phone
             << ", Факультет: " << faculty
             << ", Курс: " << course << endl;
    }
};

vector<Student> loadStudents(const string& filename) {
    vector<Student> students;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка открытия файла: " << filename << endl;
        return students;
    }

    Student s;
    while (file >> s.lastName >> s.firstName >> s.middleName
                >> s.birthYear >> s.address >> s.phone
                >> s.faculty >> s.course) {
        students.push_back(s);
    }

    file.close();
    return students;
}

vector<string> getUniqueFaculties(const vector<Student>& students) {
    vector<string> faculties;

    for (const auto& s : students) {
        bool exists = false;
        for (const auto& f : faculties) {
            if (f == s.faculty) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            faculties.push_back(s.faculty);
        }
    }

    return faculties;
}

// а) список студентов заданного факультета
void showByFaculty(const vector<Student>& students) {
    vector<string> faculties = getUniqueFaculties(students);

    if (faculties.empty()) {
        cout << "Факультеты не найдены.\n";
        return;
    }

    cout << "\nСписок факультетов:\n";
    for (size_t i = 0; i < faculties.size(); i++) {
        cout << i + 1 << ". " << faculties[i] << endl;
    }

    int facultyNumber;
    cout << "Выберите номер факультета: ";
    cin >> facultyNumber;

    if (cin.fail() || facultyNumber < 1 || facultyNumber > (int)faculties.size()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный номер факультета.\n";
        return;
    }

    string selectedFaculty = faculties[facultyNumber - 1];
    bool found = false;

    cout << "\nСписок студентов факультета " << selectedFaculty << ":\n";
    for (const auto& s : students) {
        if (s.faculty == selectedFaculty) {
            s.show();
            found = true;
        }
    }

    if (!found) {
        cout << "Студенты этого факультета не найдены.\n";
    }
}

// б) списки студентов для каждого факультета и курса
void showByFacultyAndCourse(const vector<Student>& students) {
    map<string, map<int, vector<Student>>> groups;

    for (const auto& s : students) {
        groups[s.faculty][s.course].push_back(s);
    }

    cout << "\nСписки студентов для каждого факультета и курса:\n";

    for (const auto& facultyPair : groups) {
        cout << "\nФакультет: " << facultyPair.first << endl;

        for (const auto& coursePair : facultyPair.second) {
            cout << "  Курс " << coursePair.first << ":\n";

            for (const auto& student : coursePair.second) {
                cout << "    ";
                student.show();
            }
        }
    }
}

// в) список студентов, родившихся после заданного года
void showBornAfterYear(const vector<Student>& students) {
    int yearInput;
    cout << "Введите год: ";
    cin >> yearInput;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод года.\n";
        return;
    }

    bool found = false;

    cout << "\nСписок студентов, родившихся после " << yearInput << ":\n";
    for (const auto& s : students) {
        if (s.birthYear > yearInput) {
            s.show();
            found = true;
        }
    }

    if (!found) {
        cout << "Таких студентов не найдено.\n";
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    vector<Student> students = loadStudents("students.txt");

    if (students.empty()) {
        cout << "Файл пустой или не удалось загрузить данные.\n";
        return 0;
    }

    int choice;

    do {
        cout << "\n========== МЕНЮ ==========\n";
        cout << "1. Список студентов заданного факультета\n";
        cout << "2. Списки студентов для каждого факультета и курса\n";
        cout << "3. Список студентов, родившихся после заданного года\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Неверный ввод.\n";
            continue;
        }

        switch (choice) {
            case 1:
                showByFaculty(students);
                break;
            case 2:
                showByFacultyAndCourse(students);
                break;
            case 3:
                showBornAfterYear(students);
                break;
            case 0:
                cout << "Выход из программы.\n";
                break;
            default:
                cout << "Неверный пункт меню.\n";
        }

    } while (choice != 0);

    return 0;
}