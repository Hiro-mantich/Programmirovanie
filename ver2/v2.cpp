#define _CRT_SECURE_NO_WARNINGS // Отключаем предупреждения о небезопасных функциях
#include <iostream>
#include <cstring>
#include <Windows.h>
#include <cstdlib> // Для malloc и free

const int MAX_BORROWED_BOOKS = 100; // Максимальное количество книг, которые может взять один читатель
const int MAX_BOOKS = 100;     // Максимальное количество книг в библиотеке
const int MAX_READERS = 100;     // Максимальное количество читателей в библиотеке
const int MAX_LOANS = 100;       // Максимальное количество выдач книг
const int MAX_NAME_LENGTH = 30;    // Максимальная длина имени автора, названия книги и ФИО читателя
const int MAX_AUTHORS = 100;      // Максимальное количество авторов

// Класс для хранения информации об авторе
class Author {
    char* fio;                 // ФИО автора (динамический массив)
    int birth_year;            // Год рождения автора

public:
    Author() : fio(nullptr), birth_year(0) {}

    // Геттер для получения ФИО автора
    const char* get_fio() const {
        return fio;
    }

    // Функция для добавления нового автора
    void add_Author() {
        printf("Введите ФИО автора: \n");
        while (getchar() != '\n');
        this->fio = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
        fgets(this->fio, MAX_NAME_LENGTH, stdin);
        this->fio[strcspn(this->fio, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

        printf("Введите год рождения автора: ");
        scanf("%d", &this->birth_year);

        while (getchar() != '\n'); // Очищаем буфер ввода после scanf, чтобы избежать проблем с последующими fgets
    }

    // Функция для вывода информации об авторе
    void print_Author() {
        printf("ФИО автора: %s\n", this->fio);
        printf("Год рождения автора: %d\n", this->birth_year);
    }

    ~Author() {
        if (fio) free(fio);
    }
};

// Структура для хранения информации о книге
class Book {
    char* title;               // Название книги (динамический массив)
    Author* author;            // Указатель на автора книги
    int pub_year;              // Год публикации книги
    int copies;                // Количество экземпляров книги

public:
    Book() : title(nullptr), author(nullptr), pub_year(0), copies(0) {}

    // Геттер для получения названия книги
    const char* get_title() const {
        return title;
    }

    // Метод для добавления новой книги
    void add_Book(Author** authors, int num_authors) {
        printf("Введите название книги: \n");
        while (getchar() != '\n');
        this->title = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
        fgets(this->title, MAX_NAME_LENGTH, stdin);
        this->title[strcspn(this->title, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

        printf("Выберите автора (введите номер): ");
        for (int i = 0; i <= num_authors; i++) {
            if (authors[i] != nullptr) {
                printf("%d. %s\n", i + 1, authors[i]);
            }
        }
        int author_index;
        scanf("%d", &author_index);
        this->author = authors[author_index - 1];
        printf("Введите дату публикации: ");
        scanf("%d", &this->pub_year);
        printf("Введите количество экземпляров: ");
        scanf("%d", &this->copies);
    }

    // Функция для вывода информации о книге
    void print_Book() {
        if (this->title == nullptr || this->author == nullptr) {
            printf("Ошибка: некорректные данные о книге.\n");
            return;
        }
        printf("Название книги: %s\n", this->title);
        printf("Автор книги: %s\n", this->author->get_fio());
        printf("Дата публикации: %d\n", this->pub_year);
        printf("Количество экземпляров: %d\n", this->copies);
    }

    ~Book() {
        if (title) free(title);
    }
};

// Структура для хранения информации о читателе
class Reader {
    char* fio;                 // ФИО читателя (динамический массив)
    int card_number;           // Номер читательского билета
    Book* borrowed_books[MAX_BORROWED_BOOKS]; // Массив указателей на взятые книги
    int borrowed_count;        // Количество взятых книг

public:
    Reader() : fio(nullptr), card_number(0), borrowed_count(0) {}

    void set_borrowed_count() {
        borrowed_count++;
    }

    char* get_fio() {
        return fio;
    }

    int get_card_number() {
        return card_number;
    }

    // Функция для добавления нового читателя
    void add_Reader() {
        printf("Введите ФИО читателя: \n");
        while (getchar() != '\n');
        this->fio = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
        fgets(this->fio, MAX_NAME_LENGTH, stdin);
        this->fio[strcspn(this->fio, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

        printf("Введите номер читательского билета: ");
        scanf("%d", &this->card_number);
        this->borrowed_count = 0;
    }

    // Функция для вывода информации о читателе
    void print_Reader() {
        if (this->fio == nullptr) {
            printf("Ошибка: некорректные данные о читателе.\n");
            return;
        }
        printf("ФИО читателя: %s\n", this->fio);
        printf("Номер читательского билета: %d\n", this->card_number);
        printf("Количество взятых книг: %d\n", this->borrowed_count);
    }

    ~Reader() {
        if (fio) free(fio);
    }
};

//КЛАСС ЛОАН
class Loan {
    Book* book;                // Указатель на книгу
    Reader* reader;            // Указатель на читателя
    char issue_date[10];       // Дата выдачи книги
    char return_date[10];      // Дата возврата книги

public:
    Loan() : book(nullptr), reader(nullptr) {}

    Loan(Book* book, Reader* reader, const char* issue_date, const char* return_date)
        : book(book), reader(reader) {
        strncpy(this->issue_date, issue_date, 9);
        this->issue_date[9] = '\0';
        strncpy(this->return_date, return_date, 9);
        this->return_date[9] = '\0';
    }

    char* get_issue_date() {
        return issue_date;
    }

    char* get_return_date() {
        return return_date;
    }

    // Функция для вывода информации о выдаче книги
    void print_Loan() {
        if (book == nullptr || reader == nullptr || book->get_title() == nullptr || reader->get_fio() == nullptr) {
            printf("Ошибка: некорректные данные о выдаче.\n");
            return;
        }
        printf("Название выданной книги: %s\n", book->get_title());
        printf("Номер читательского билета: %d\n", reader->get_card_number());
        printf("Дата выдачи книги: %s\n", this->issue_date);
        printf("Дата возврата книги: %s\n", this->return_date);
    }
};

// Структура для хранения информации о библиотеке
class Library {
    Author* authors[MAX_AUTHORS];
    Book* books[MAX_BOOKS];    // Массив указателей на книги
    int book_count;            // Количество книг в библиотеке
    Reader* readers[MAX_READERS]; // Массив указателей на читателей
    int reader_count;          // Количество читателей в библиотеке
    Loan* loans[MAX_LOANS];    // Массив указателей на выдачи
    int loan_count;            // Количество выдач книг

public:
    Library() : book_count(0), reader_count(0), loan_count(0) {
        for (int i = 0; i < MAX_AUTHORS; i++) {
            authors[i] = nullptr;
        }
        for (int i = 0; i < MAX_BOOKS; i++) {
            books[i] = nullptr;
        }
        for (int i = 0; i < MAX_READERS; i++) {
            readers[i] = nullptr;
        }
        for (int i = 0; i < MAX_LOANS; i++) {
            loans[i] = nullptr;
        }
    }

    // Геттер для получения количества книг
    int get_book_count() const {
        return book_count;
    }

    // Геттер для получения количества читателей
    int get_reader_count() const {
        return reader_count;
    }

    // Геттер для получения книги по индексу
    Book* get_books(int i) {
        return books[i];
    }

    // Геттер для получения читателя по индексу
    Reader* get_reader(int i) {
        return readers[i];
    }

    // Функция для вывода всей информации о библиотеке
    void print_Library() {
        printf("Список авторов:\n");
        for (int i = 0; i < MAX_AUTHORS; i++) {
            if (authors[i] != nullptr && authors[i]->get_fio() != nullptr) {
                authors[i]->print_Author();
            }
        }

        printf("Список книг:\n");
        for (int i = 0; i < book_count; i++) {
            if (books[i] != nullptr && books[i]->get_title() != nullptr) {
                books[i]->print_Book();
            }
        }

        printf("Список читателей:\n");
        for (int i = 0; i < reader_count; i++) {
            if (readers[i] != nullptr && readers[i]->get_fio() != nullptr) {
                readers[i]->print_Reader();
            }
        }

        printf("Список выдач:\n");
        for (int i = 0; i < loan_count; i++) {
            if (loans[i] != nullptr && loans[i]->get_issue_date() != nullptr) {
                loans[i]->print_Loan();
            }
        }
    }

    // Функция для добавления нового автора
    void add_Author() {
        if (book_count < MAX_AUTHORS) {
            authors[book_count] = new Author();
            authors[book_count]->add_Author();
        }
        else {
            printf("Ошибка: достигнуто максимальное количество авторов.\n");
        }
    }

    // Функция для добавления новой книги
    void add_Book() {
        if (book_count < MAX_BOOKS) {
            books[book_count] = new Book();
            books[book_count]->add_Book(authors, book_count);
            book_count++;
        }
        else {
            printf("Ошибка: достигнуто максимальное количество книг.\n");
        }
    }

    // Функция для добавления нового читателя
    void add_Reader() {
        if (reader_count < MAX_READERS) {
            readers[reader_count] = new Reader();
            readers[reader_count]->add_Reader();
            reader_count++;
        }
        else {
            printf("Ошибка: достигнуто максимальное количество читателей.\n");
        }
    }

    // Функция для добавления новой выдачи книги
    void add_Loan() {
        if (loan_count < MAX_LOANS) {
            if (book_count == 0) {
                printf("Ошибка: количество книг равно 0. Добавьте книги в библиотеку.\n");
                return;
            }

            printf("Выберите книгу (введите номер): ");
            for (int i = 0; i < book_count; i++) {
                if (books[i] != nullptr && books[i]->get_title() != nullptr) {
                    printf("%d. %s\n", i + 1, books[i]->get_title());
                }
            }
            int book_index;
            scanf("%d", &book_index);
            if (book_index < 1 || book_index > book_count || books[book_index - 1] == nullptr) {
                printf("Ошибка: неверный номер книги.\n");
                return;
            }
            Book* selected_book = books[book_index - 1];

            if (reader_count == 0) {
                printf("Ошибка: количество читателей равно 0. Добавьте читателей в библиотеку.\n");
                return;
            }

            printf("Выберите читателя (введите номер): ");
            for (int i = 0; i < reader_count; i++) {
                if (readers[i] != nullptr && readers[i]->get_fio() != nullptr) {
                    printf("%d. %s\n", i + 1, readers[i]->get_fio());
                }
            }
            int reader_index;
            scanf("%d", &reader_index);
            if (reader_index < 1 || reader_index > reader_count || readers[reader_index - 1] == nullptr) {
                printf("Ошибка: неверный номер читателя.\n");
                return;
            }
            Reader* selected_reader = readers[reader_index - 1];

            char issue_date[11];
            char return_date[11];
            printf("Введите дату выдачи книги (дд.мм.гггг): ");
            scanf("%s", issue_date);
            printf("Введите дату возврата книги (дд.мм.гггг): ");
            scanf("%s", return_date);

            loans[loan_count] = new Loan(selected_book, selected_reader, issue_date, return_date);
            loan_count++;

            selected_reader->set_borrowed_count();
        }
        else {
            printf("Ошибка: достигнуто максимальное количество выдач.\n");
        }
    }

    ~Library() {
        // Освобождаем память для авторов
        for (int i = 0; i < MAX_AUTHORS; i++) {
            if (authors[i] != nullptr) {
                delete authors[i];
                authors[i] = nullptr;
            }
        }

        // Освобождаем память для книг
        for (int i = 0; i < book_count; i++) {
            if (books[i] != nullptr) {
                delete books[i];
                books[i] = nullptr;
            }
        }

        // Освобождаем память для читателей
        for (int i = 0; i < reader_count; i++) {
            if (readers[i] != nullptr) {
                delete readers[i];
                readers[i] = nullptr;
            }
        }

        // Освобождаем память для выдач
        for (int i = 0; i < loan_count; i++) {
            if (loans[i] != nullptr) {
                delete loans[i];
                loans[i] = nullptr;
            }
        }
    }
};

int main() {
    SetConsoleCP(1251);          // Устанавливаем кодировку ввода
    SetConsoleOutputCP(1251);    // Устанавливаем кодировку вывода

    Library library;

    printf("БИБЛИОТЕЧНЫЙ УЧЁТ\n");

    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Добавить автора\n");
        printf("2. Добавить книгу\n");
        printf("3. Добавить читателя\n");
        printf("4. Выдать книгу\n");
        printf("5. Просмотреть все данные\n");
        printf("6. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            library.add_Author(); // Добавляем нового автора
            break;
        case 2:
            library.add_Book();     // Добавляем новую книгу
            break;
        case 3:
            library.add_Reader(); // Добавляем нового читателя
            break;
        case 4:
            library.add_Loan();       // Добавляем новую выдачу книги
            break;
        case 5:
            library.print_Library();                   // Выводим все данные о библиотеке
            break;
        case 6:
            printf("Выход из программы.\n");           // Выход из программы
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n"); // Обработка неверного выбора
        }
    } while (choice != 6);

    return 0;
}