#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <clocale>

using namespace std;

// Цвета текста
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"

// Объявление переменных
const int MAX_STUDENTS_IN_ARRAY = 100;
const int MAX_NUMBERS_IN_ARRAY = 100;
const int grades_count = 8;
int current_num_students;
int current_num_numbers;
string file_directory = "C:/students_list.txt";     // Формат текстового файла - ANSI !
string file_directory_phone = "C:/phone_book.txt";  // Формат текстового файла - ANSI !

// Флаги
bool is_need_array_refresh = true;

// Структуры
struct Student {
    string full_name;          // ФИО студента
    char sex;                  // Пол (М/Ж)
    int group;                 // Номер группы
    int group_id;              // Номер студента в списке группы
    int grades[grades_count];  // Оценки за последный семестр
};

struct Phone {
    string full_name;
    string phone_num;
    string phone_type;
    string phone_operator;
    string city;
};

// Объявление массивов
struct Student students_list[MAX_STUDENTS_IN_ARRAY];
struct Phone phone_book[MAX_NUMBERS_IN_ARRAY];

//Прототипы функций
int menu_commands(int choice);

// Функции
void separation() {
    cout << "\n";
}

void waiting() {
    separation();
    separation();
    cout << RED << "Нажмите Enter для продолжения..." << RESET << endl;
    cin.ignore();
    cin.get();
}

void clear_screen() {
    system("cls");
}

void students_counter() {
    int count_str = 0;
    string temp_data;

    ifstream List;
    List.open(file_directory);
    if (List.is_open()) {
        while (!List.eof()) {
            getline(List, temp_data);
            count_str++;
        }

        List.close();
        current_num_students = (count_str + 1) / 6;
    }
    else {
        cout << RED << "Не удалось открыть файл! Убедитесь, что файл находится по адресу " << file_directory << RESET << endl;
    }
}

void students_array_filling() {
    students_counter();
    if (current_num_students != 0) {
        ifstream List;
        List.open(file_directory);
        if (List.is_open()) {
            for (int i = 0; i < current_num_students; i++) {
                getline(List, students_list[i].full_name);
                List >> students_list[i].sex;
                List >> students_list[i].group;
                List >> students_list[i].group_id;
                for (int k = 0; k < grades_count; k++) {
                    List >> students_list[i].grades[k];
                }
                List.ignore(1000, '\n');
                string temp;
                getline(List, temp); // пропуск пустой строки
            }
            List.close();
            is_need_array_refresh = false;
        }
        else {
            cout << RED << "Не удалось открыть файл! Убедитесь, что файл находится по адресу " << file_directory << RESET << endl;
        }
    }
    else {
        cout << RED << "В файле нет данных о студентах!" << RESET << endl;
    }
}

void student_output(struct Student* student_profile) {
    cout << GREEN << "ФИО: " << RESET << student_profile->full_name << endl;
    cout << GREEN << "Пол: " << RESET << student_profile->sex << endl;
    cout << GREEN << "Группа: " << RESET << student_profile->group << endl;
    cout << GREEN << "Номер в группе: " << RESET << student_profile->group_id << endl;
    cout << GREEN << "Оценки: " << RESET;
    for (int i = 0; i < grades_count; i++) {
        cout << student_profile->grades[i] << " ";
    }
    separation();
    separation();
}

void student_array_output() {
    for (int i = 0; i < current_num_students; i++) {
        struct Student* p_student = &students_list[i];
        student_output(p_student);
    }
}

void students_list_input() {
    students_counter();
    if (current_num_students != 0) {
        cout << GREEN << "Данные всех " << RED << current_num_students << GREEN << " студентов:" << RESET << endl;
        separation();
        students_array_filling();
        student_array_output();
    }
    else {
        cout << RED << "В файле нет данных о студентах!" << RESET << endl;
    }
}

void make_new_student() {
    if (is_need_array_refresh) {
        students_array_filling();
    }
    cout << BLUE << "Добавление нового студента в список." << RESET << endl;
    separation();
    students_counter();
    int num = current_num_students;

    cin.ignore();
    cout << GREEN << "Введите ФИО: " << RESET;
    getline(cin, students_list[num].full_name);
    separation();

    cout << GREEN << "Введите пол (М/Ж): " << RESET;
    cin >> students_list[num].sex;
    separation();

    cout << GREEN << "Введите номер группы: " << RESET;
    cin >> students_list[num].group;
    separation();

    cout << GREEN << "Введите номер студента в списке: " << RESET;
    cin >> students_list[num].group_id;
    separation();

    cout << GREEN << "Введите 8 оценок за последний семестр: " << RESET;
    for (int i = 0; i < grades_count; i++) {
        cin >> students_list[num].grades[i];
    }

    bool is_have_two = false;
    for (int i = 0; i < grades_count; i++) {
        if (students_list[num].grades[i] == 2) {
            is_have_two = true;
            break;
        }
    }
    if (!is_have_two) {
        ofstream List;
        List.open(file_directory, ios::app);
        if (List.is_open()) {
            List << "\n" << "\n";
            List << students_list[num].full_name << "\n";
            List << students_list[num].sex << "\n";
            List << students_list[num].group << "\n";
            List << students_list[num].group_id << "\n";
            for (int i = 0; i < grades_count; i++) {
                List << students_list[num].grades[i] << " ";
            }
            List.close();

            separation();
            cout << GREEN << "Данные успешно записаны в файл!" << RESET << endl;
            is_need_array_refresh = false;
            students_counter();
        }
        else {
            cout << RED << "Не удалось открыть файл! Убедитесь, что файл находится по адресу " << file_directory << RESET << endl;
        }
    }
    else {
        cout << RED << "Студент имеет 2 за семестр и будет отчислен. Данные не будут сохранены.";
    }
}

void save_array_to_file() {
    ofstream List;
    List.open(file_directory, ios::trunc);
    if (List.is_open()) {
        for (int i = 0; i < current_num_students; i++) {
            if (i >= 1) {
                List << "\n\n";
            }
            List << students_list[i].full_name << "\n";
            List << students_list[i].sex << "\n";
            List << students_list[i].group << "\n";
            List << students_list[i].group_id << "\n";
            for (int j = 0; j < grades_count; j++) {
                List << students_list[i].grades[j] << " ";
            }
        }
        List.close();

        separation();
        cout << GREEN << "Данные успешно записаны в файл!" << RESET << endl;
        is_need_array_refresh = true;
    }
    else {
        separation();
        cout << RED << "Не удалось открыть файл! Запустите программу с правами администратора и убедитесь, что файл находится по адресу " << file_directory << RESET << endl;
    }
}

void edit_student() {
    if (is_need_array_refresh) {
        students_array_filling();
    }
    cout << BLUE << "Изменение данных студента в списке." << RESET << endl;
    separation();
    students_counter();

    cout << GREEN << "Введите ФИО студента: " << RESET;
    string student_name;
    cin.ignore();
    getline(cin, student_name);
    cout << GREEN << "Введите группу студента: " << RESET;
    int student_group;
    cin >> student_group;
    clear_screen();
    int edit_num;
    int temp = 0;
    for (int i = 0; i < current_num_students; i++) {
        if (students_list[i].full_name == student_name && students_list[i].group == student_group) {
            temp++;
            edit_num = i;
        }
    }
    if (temp == 1) {
        cout << GREEN << "Студент найден! Данные о студенте до изменений:" << RESET << endl;
        student_output(&students_list[edit_num]);
        separation();
        cout << RED << "Изменение данных студента:" << RESET << endl;

        cin.ignore();
        cout << GREEN << "Введите ФИО: " << RESET;
        getline(cin, students_list[edit_num].full_name);
        separation();

        cout << GREEN << "Введите пол (М/Ж): " << RESET;
        cin >> students_list[edit_num].sex;
        separation();

        cout << GREEN << "Введите номер группы: " << RESET;
        cin >> students_list[edit_num].group;
        separation();

        cout << GREEN << "Введите номер студента в списке: " << RESET;
        cin >> students_list[edit_num].group_id;
        separation();

        cout << GREEN << "Введите 8 оценок за последний семестр: " << RESET;
        for (int i = 0; i < grades_count; i++) {
            cin >> students_list[edit_num].grades[i];
        }

        bool is_have_two = false;
        for (int i = 0; i < grades_count; i++) {
            if (students_list[edit_num].grades[i] == 2) {
                is_have_two = true;
                break;
            }
        }

        if (!is_have_two) {
            save_array_to_file();
        }
        else {
            cout << RED << "Студент имеет 2 за семестр и будет отчислен. Данные не будут сохранены.";
        }
    }
    else {
        cout << RED << "Студент не найден в списке! Проверьте ФИО и группу." << RESET;
    }
}

void students_output_group() {
    if (is_need_array_refresh) {
        students_array_filling();
    }
    cout << BLUE << "Вывод данных о студентах выбранной группы" << RESET << endl;
    separation();
    cout << GREEN << "Введите номер группы: " << RESET;
    int group_choice;
    int temp = 0;
    cin >> group_choice;
    separation();
    for (int i = 0; i < current_num_students; i++) {
        if (students_list[i].group == group_choice) {
            student_output(&students_list[i]);
            temp++;
        }
    }
    separation();
    if (temp != 0) {
        cout << GREEN << "Успешно выведены данные " << RED << temp << GREEN << " студентов группы " << RED << group_choice << RESET;
    }
    else {
        cout << RED << "В списке нет данных о студентах из группы " << GREEN << group_choice << RESET;
    }
}

float average_grades(int* p_grades) {
    float sum = 0;
    for (int i = 0; i < grades_count; i++) {
        sum += p_grades[i];
    }
    float temp = sum / grades_count;
    return temp;
}

void students_output_top() {
    if (is_need_array_refresh) {
        students_array_filling();
    }

    cout << BLUE << "Топ студентов по среднему баллу за семестр:" << RESET << endl;
    separation();

    struct AverageGrades {
        struct Student* p_stud;
        float avg_grades;
    };

    struct AverageGrades students_top[MAX_STUDENTS_IN_ARRAY];

    for (int i = 0; i < current_num_students; i++) {
        students_top[i].p_stud = &students_list[i];
        students_top[i].avg_grades = average_grades(students_list[i].grades);
    }

    for (int i = 0; i < current_num_students - 1; i++) {
        for (int j = 0; j < current_num_students - i - 1; j++) {
            if (students_top[j].avg_grades < students_top[j + 1].avg_grades) {
                swap(students_top[j], students_top[j + 1]);
            }
        }
    }

    int top_size = 10;
    for (int i = 0; i < top_size; i++) {
        cout << RED << right << setw(2) << i + 1 << ") " << GREEN << left << setw(35) << students_top[i].p_stud->full_name << " | " << students_top[i].p_stud->group << " | Средний балл: " << students_top[i].avg_grades << RESET << endl;
    }
    separation();
}

void students_output_sex() {
    if (is_need_array_refresh) {
        students_array_filling();
    }

    cout << BLUE << "Вывод количества студентов мужского и женского пола" << RESET << endl;
    separation();
    int man_count = 0;
    int woman_count = 0;
    for (int i = 0; i < current_num_students; i++) {
        if (students_list[i].sex == 'М') {
            man_count++;
        }
        if (students_list[i].sex == 'Ж') {
            woman_count++;
        }
    }
    cout << GREEN << "Студентов мужского пола: " << RED << man_count << RESET << endl;
    cout << GREEN << "Студентов женского пола: " << RED << woman_count << RESET << endl;

}

void students_output_grades() {
    if (is_need_array_refresh) {
        students_array_filling();
    }
    cout << BLUE << "Вывод данных о студентах по оценкам" << RESET << endl;
    separation();
    cout << GREEN << "Выберите категорию: \n\n"
        "1) Cтуденты, которые имеют оценку «удовлетворительно» (не получают стипендию)\n"
        "2) Cтуденты, которые учатся только на «хорошо» и «отлично»\n"
        "3) Cтуденты, которые учатся только на «отлично»\n"
        << RED << "Выбор: " << RESET;
    int grades_choice;
    cin >> grades_choice;
    int temp = 0;
    switch (grades_choice) {
    case 1: {
        clear_screen();
        cout << BLUE << "Cтуденты, которые имеют оценку «удовлетворительно» (не получают стипендию)" << RESET << endl;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_students; i++) {
            int count_3 = 0;
            for (int j = 0; j < grades_count; j++) {
                if (students_list[i].grades[j] == 3) {
                    count_3++;
                }
            }
            if (count_3 != 0) {
                student_output(&students_list[i]);
                temp++;
            }
        }
        if (temp != 0) {
            separation();
            cout << GREEN << "Под выбранную категорию подходят " << RED << temp << GREEN << " студентов" << RESET << endl;
        }
        else {
            cout << RED << "В списке нет студентов, подходящих под выбранную категорию!";
        }
        break;
    }

    case 2: {
        clear_screen();
        cout << BLUE << "Cтуденты, которые учатся только на «хорошо» и «отлично»" << RESET << endl;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_students; i++) {
            int count_3 = 0;
            int count_5 = 0;
            for (int j = 0; j < grades_count; j++) {
                if (students_list[i].grades[j] == 3) {
                    count_3++;
                }
                if (students_list[i].grades[j] == 5) {
                    count_5++;
                }
            }
            if (count_3 == 0 && count_5 < grades_count) {
                student_output(&students_list[i]);
                temp++;
            }
        }
        if (temp != 0) {
            separation();
            cout << GREEN << "Под выбранную категорию подходят " << RED << temp << GREEN << " студентов" << RESET << endl;
        }
        else {
            cout << RED << "В списке нет студентов, подходящих под выбранную категорию!";
        }
        break;
    }

    case 3: {
        clear_screen();
        cout << BLUE << "Cтуденты, которые учатся только на «отлично»" << RESET << endl;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_students; i++) {
            int count_5 = 0;
            for (int j = 0; j < grades_count; j++) {
                if (students_list[i].grades[j] == 5) {
                    count_5++;
                }
            }
            if (count_5 == grades_count) {
                student_output(&students_list[i]);
                temp++;
            }
        }
        if (temp != 0) {
            separation();
            cout << GREEN << "Под выбранную категорию подходят " << RED << temp << GREEN << " студентов" << RESET << endl;
        }
        else {
            cout << RED << "В списке нет студентов, подходящих под выбранную категорию!";
        }
        break;
    }

    default:
        separation();
        cout << RED << "Ошибка! Введите номер категории от 1 до 3!\n" << RESET << endl;
    }
}

void students_output_group_id() {
    if (is_need_array_refresh) {
        students_array_filling();
    }
    cout << BLUE << "Вывод данных о студентах с нужным номером в списке" << RESET << endl;
    separation();
    cout << GREEN << "Введите номер cтудента(-ов) в списке группы: " << RESET;
    int group_id_choice;
    int temp = 0;
    cin >> group_id_choice;
    separation();
    for (int i = 0; i < current_num_students; i++) {
        if (students_list[i].group_id == group_id_choice) {
            student_output(&students_list[i]);
            temp++;
        }
    }
    separation();
    if (temp != 0) {
        cout << GREEN << "Успешно выведены данные " << RED << temp << GREEN << " студентов с номером " << RED << group_id_choice << RESET;
    }
    else {
        cout << RED << "В списке нет данных студентом с номером " << GREEN << group_id_choice << RESET;
    }
}

void phone_output(struct Phone* phone_profile) {
    cout << GREEN << "ФИО: " << RESET << phone_profile->full_name << endl;
    cout << GREEN << "Номер: " << RESET << phone_profile->phone_num << endl;
    cout << GREEN << "Тип телефона: " << RESET << phone_profile->phone_type << endl;
    cout << GREEN << "Оператор: " << RESET << phone_profile->phone_operator << endl;
    cout << GREEN << "Город: " << RESET << phone_profile->city << endl;
    separation();
}

void phone_array_output() {
    for (int i = 0; i < current_num_numbers; i++) {
        struct Phone* p_phone = &phone_book[i];
        phone_output(p_phone);
    }
}

void numbers_counter() {
    int count_str = 0;
    string temp_data;

    ifstream List;
    List.open(file_directory_phone);
    if (List.is_open()) {
        while (!List.eof()) {
            getline(List, temp_data);
            count_str++;
        }

        List.close();
        current_num_numbers = (count_str + 1) / 6;
    }
    else {
        cout << RED << "Не удалось открыть файл! Убедитесь, что файл находится по адресу " << file_directory_phone << RESET << endl;
    }
}

void phone_array_filling() {
    numbers_counter();
    if (current_num_numbers != 0) {
        ifstream List;
        List.open(file_directory_phone);
        if (List.is_open()) {
            for (int i = 0; i < current_num_numbers; i++) {
                getline(List, phone_book[i].full_name);
                getline(List, phone_book[i].phone_num);
                getline(List, phone_book[i].phone_type);
                getline(List, phone_book[i].phone_operator);
                getline(List, phone_book[i].city);
                string temp;
                getline(List, temp); // пропуск пустой строки
            }
            List.close();
        }
        else {
            cout << RED << "Не удалось открыть файл! Убедитесь, что файл находится по адресу " << file_directory_phone << RESET << endl;
        }
    }
    else {
        cout << RED << "В файле нет данных!" << RESET << endl;
    }
}

void phone_book_menu() {
    phone_array_filling();
    cout << BLUE << "Телефонный справочник" << RESET << endl;
    separation();
    cout << GREEN << "Выберите операцию: \n\n"
        "1) Вывести всех абонентов указанного оператора\n"
        "2) Вывести список контактов для смс-рассылки\n"
        "3) Найти человека по номеру\n"
        << RED << "Выбор: " << RESET;
    int phone_choice;
    cin >> phone_choice;
    clear_screen();
    switch (phone_choice) {
    case 1: {
        cout << BLUE << "Все абоненты указанного оператора" << RESET << endl;
        separation();
        cout << GREEN << "Сейчас в списке следующие операторы: " << RESET << "МТС, Ростелеком, МГТС, МегаФон, Билайн, Дом.ру, Tele2, Башинформсвязь, Таттелеком, Yota, Интерсвязь" << endl;
        cout << GREEN << "Введите название оператора: " << RESET;
        string operator_search;
        cin >> operator_search;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_numbers; i++) {
            if (phone_book[i].phone_operator == operator_search) {
                phone_output(&phone_book[i]);
                temp++;
            }
        }
        if (temp != 0) {
            cout << GREEN << "Успешный вывод!" << RESET;
        }
        else {
            cout << RED << "Не удалось найти абонентов с заданным оператором или название оператора введено неверно!" << RESET;
        }
        break;
    }

    case 2: {
        cout << BLUE << "Список контактов для смс-рассылки:" << RESET << endl;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_numbers; i++) {
            if (phone_book[i].phone_type == "мобильный") {
                phone_output(&phone_book[i]);
                temp++;
            }
        }
        if (temp != 0) {
            cout << GREEN << "Список контактов для смс-рассылки успешно выведен!" << RESET;
        }
        else {
            cout << RED << "В списке нет подходящий номеров для смс-рассылки!" << RESET;
        }
        break;
    }

    case 3: {
        cout << BLUE << "Найти человека по номеру" << RESET << endl;
        separation();
        cout << GREEN << "Введите номер телефона (без +): " << RESET;
        string num_search;
        cin >> num_search;
        separation();
        int temp = 0;
        for (int i = 0; i < current_num_numbers; i++) {
            if (phone_book[i].phone_num == num_search) {
                phone_output(&phone_book[i]);
                temp++;
                break;
            }
        }
        if (temp == 1) {
            cout << GREEN << "Человек успешно найден!" << RESET;
        }
        else {
            cout << RED << "Человека с таким номером нет в списке!" << RESET;
        }
        break;
    }
    }
}

void command_choice() {
    cout << RED << "Введите номер команды: " << RESET;
    int user_choice;
    cin >> user_choice;
    separation();
    menu_commands(user_choice);
}

int menu_commands(int choice) {
    switch (choice) {
    case 0:
        students_counter();
        cout << GREEN << "Сейчас в списке данные о " << RED << current_num_students << GREEN << " студентах" << RESET << endl;
        waiting();
        return 0;
        break;
    case 1:
        clear_screen();
        make_new_student();
        is_need_array_refresh = true;
        waiting();
        return 0;
        break;
    case 2:
        clear_screen();
        edit_student();
        waiting();
        return 0;
        break;
    case 3:
        clear_screen();
        students_list_input();
        waiting();
        return 0;
        break;
    case 4:
        clear_screen();
        students_output_group();
        waiting();
        return 0;
        break;
    case 5:
        clear_screen();
        students_output_top();
        waiting();
        return 0;
        break;
    case 6:
        clear_screen();
        students_output_sex();
        waiting();
        return 0;
        break;
    case 7:
        clear_screen();
        students_output_grades();
        waiting();
        return 0;
        break;
    case 8:
        clear_screen();
        students_output_group_id();
        waiting();
        return 0;
        break;
    case 9:
        clear_screen();
        phone_book_menu();
        waiting();
        return 0;
        break;
    default:
        cout << RED << "Ошибка! Введите номер команды от 1 до 7\n" << RESET << endl;
        break;
    }
}

void lab_page() {
    cout << "|=========================================================|" << endl;
    cout << "|                  " << BLUE << "Лабораторная работа 1" << RESET << "                  |" << endl;
    cout << "|=========================================================|" << endl;
    cout << "| " << GREEN << "1) Создать новую запись о студенте" << RESET << "                      |" << endl;
    cout << "| " << GREEN << "2) Внести изменения в запись о студенте" << RESET << "                 |" << endl;
    cout << "| " << GREEN << "3) Вывод всех данных о студентах" << RESET << "                        |" << endl;
    cout << "| " << GREEN << "4) Вывод данных о студентах выбранной группы" << RESET << "            |" << endl;
    cout << "| " << GREEN << "5) Вывод топа самых успешных студентов" << RESET << "                  |" << endl;
    cout << "| " << GREEN << "6) Вывод количества студентов мужского и женского пола" << RESET << "  |" << endl;
    cout << "| " << GREEN << "7) Вывод данных о студентах по оценкам" << RESET << "                  |" << endl;
    cout << "| " << GREEN << "8) Вывод данных о студентах с нужным номером в списке" << RESET << "   |" << endl;
    cout << "| " << GREEN << "9) Телефонный справочник (ИДЗ)" << RESET << "                          |" << endl;
    cout << "|                                                         |" << endl;
    cout << "| " << RED << "0) Вывести текущее количество студентов в файле" << RESET << "         |" << endl;
    cout << "|=========================================================|" << endl;
    cout << "|                  " << BLUE << "Лабораторная работа 1" << RESET << "                  |" << endl;
    cout << "|=========================================================|" << endl;
    cout << endl;
}
// Главная функция
int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");
    while (true) {
        lab_page();
        command_choice();
        clear_screen();
    }
    return 0;
}