#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>

using namespace std;

class Student {
private:
    string lastName;
    string firstName;
    string middleName;
    int birthYear;
    string address;
    string phone;
    string faculty;
    int course;

public:
    // set
    void setData(string ln, string fn, string mn, int by,
                 string addr, string ph, string fac, int crs) {
        lastName = ln;
        firstName = fn;
        middleName = mn;
        birthYear = by;
        address = addr;
        phone = ph;
        faculty = fac;
        course = crs;
    }

    // get
    string getLastName() const { return lastName; }
    string getFirstName() const { return firstName; }
    string getMiddleName() const { return middleName; }
    int getBirthYear() const { return birthYear; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getFaculty() const { return faculty; }
    int getCourse() const { return course; }

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
        cout << "Ошибка открытия файла!\n";
        return students;
    }

    string ln, fn, mn, addr, ph, fac;
    int by, crs;

    while (file >> ln >> fn >> mn >> by >> addr >> ph >> fac >> crs) {
        Student s;
        s.setData(ln, fn, mn, by, addr, ph, fac, crs);
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
            if (f == s.getFaculty()) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            faculties.push_back(s.getFaculty());
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

    int choice;
    cout << "Выберите номер факультета: ";
    cin >> choice;

    if (cin.fail() || choice < 1 || choice > (int)faculties.size()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный номер факультета.\n";
        return;
    }

    string selectedFaculty = faculties[choice - 1];
    bool found = false;

    cout << "\nСписок студентов факультета " << selectedFaculty << ":\n";
    for (const auto& s : students) {
        if (s.getFaculty() == selectedFaculty) {
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
        groups[s.getFaculty()][s.getCourse()].push_back(s);
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
void showAfterYear(const vector<Student>& students) {
    int year;
    cout << "Введите год: ";
    cin >> year;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Неверный ввод года.\n";
        return;
    }

    bool found = false;

    cout << "\nСписок студентов, родившихся после " << year << ":\n";
    for (const auto& s : students) {
        if (s.getBirthYear() > year) {
            s.show();
            found = true;
        }
    }

    if (!found) {
        cout << "Нет студентов\n";
    }
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    vector<Student> students = loadStudents("students.txt");

    if (students.empty()) {
        cout << "Файл пуст или ошибка\n";
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
                showAfterYear(students);
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