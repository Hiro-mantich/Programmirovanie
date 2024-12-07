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

// Структура для хранения информации об авторе
struct Author {
    char* fio;                 // ФИО автора (динамический массив)
    int birth_year;            // Год рождения автора
} author[MAX_AUTHORS];         // Массив авторов

// Структура для хранения информации о книге
struct Book {
    char* title;               // Название книги (динамический массив)
    Author* author;            // Указатель на автора книги
    int pub_year;              // Год публикации книги
    int copies;                // Количество экземпляров книги
} book[MAX_BOOKS];             // Массив книг

// Структура для хранения информации о читателе
struct Reader {
    char* fio;                 // ФИО читателя (динамический массив)
    int card_number;           // Номер читательского билета
    Book* borrowed_books[MAX_BORROWED_BOOKS]; // Массив указателей на взятые книги
    int borrowed_count;        // Количество взятых книг
} reader[MAX_READERS];         // Массив читателей

// Структура для хранения информации о выдаче книги
struct Loan {
    Book* book;                // Указатель на книгу
    Reader* reader;            // Указатель на читателя
    char issue_date[10];       // Дата выдачи книги
    char return_date[10];      // Дата возврата книги
} loan[MAX_LOANS];             // Массив выдач

// Структура для хранения информации о библиотеке
struct Library {
    Book* books[MAX_BOOKS];    // Массив указателей на книги
    int book_count;            // Количество книг в библиотеке
    Reader* readers[MAX_READERS]; // Массив указателей на читателей
    int reader_count;          // Количество читателей в библиотеке
    Loan* loans[MAX_LOANS];    // Массив указателей на выдачи
    int loan_count;            // Количество выдач книг
} library;                    // Объект библиотеки

// Функция для добавления нового автора
void add_Author(Author* author) {
    printf("Введите ФИО автора: \n");
    while (getchar() != '\n');
    author->fio = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    fgets(author->fio, MAX_NAME_LENGTH, stdin);
    author->fio[strcspn(author->fio, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

    printf("Введите год рождения автора: ");
    scanf("%d", &author->birth_year);

    while (getchar() != '\n'); // Очищаем буфер ввода после scanf, чтобы избежать проблем с последующими fgets
}

// Функция для вывода информации об авторе
void print_Author(Author author) {
    printf("ФИО автора: %s\n", author.fio);
    printf("Год рождения автора: %d\n", author.birth_year);
}

// Функция для добавления новой книги
void add_Book(Book* book) {
    printf("Введите название книги: \n");
    while (getchar() != '\n');
    book->title = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    fgets(book->title, MAX_NAME_LENGTH, stdin);
    book->title[strcspn(book->title, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

    printf("Выберите автора (введите номер): ");
    for (int i = 0; i < MAX_AUTHORS; i++) {
        if (author[i].fio != NULL) {
            printf("%d. %s\n", i + 1, author[i].fio);
        }
    }
    int author_index;
    scanf("%d", &author_index);
    book->author = &author[author_index - 1];
    printf("Введите дату публикации: ");
    scanf("%d", &book->pub_year);
    printf("Введите количество экземпляров: ");
    scanf("%d", &book->copies);
}

// Функция для вывода информации о книге
void print_Book(Book book) {
    if (book.title == NULL || book.author == NULL) {
        printf("Ошибка: некорректные данные о книге.\n");
        return;
    }
    printf("Название книги: %s\n", book.title);
    printf("Автор книги: %s\n", book.author->fio);
    printf("Дата публикации: %d\n", book.pub_year);
    printf("Количество экземпляров: %d\n", book.copies);
}

// Функция для добавления нового читателя
void add_Reader(Reader* reader) {
    printf("Введите ФИО читателя: \n");
    while (getchar() != '\n');
    reader->fio = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    fgets(reader->fio, MAX_NAME_LENGTH, stdin);
    reader->fio[strcspn(reader->fio, "\n")] = '\0'; // Удаляем символ новой строки, который остается после fgets

    printf("Введите номер читательского билета: ");
    scanf("%d", &reader->card_number);
    reader->borrowed_count = 0;
}

// Функция для вывода информации о читателе
void print_Reader(Reader reader) {
    if (reader.fio == NULL) {
        printf("Ошибка: некорректные данные о читателе.\n");
        return;
    }
    printf("ФИО читателя: %s\n", reader.fio);
    printf("Номер читательского билета: %d\n", reader.card_number);
    printf("Количество взятых книг: %d\n", reader.borrowed_count);
}

// Функция для добавления новой выдачи книги
void add_Loan(Loan* loan) {
    printf("Выберите книгу (введите номер): ");
    for (int i = 0; i < library.book_count; i++) {
        if (book[i].title != NULL) {
            printf("%d. %s\n", i + 1, book[i].title);
        }
    }
    int book_index;
    scanf("%d", &book_index);
    loan->book = library.books[book_index - 1];

    printf("Выберите читателя (введите номер): ");
    for (int i = 0; i < library.reader_count; i++) {
        if (reader[i].fio != NULL) {
            printf("%d. %s\n", i + 1, reader[i].fio);
        }
    }
    int reader_index;
    scanf("%d", &reader_index);
    loan->reader = library.readers[reader_index - 1];

    printf("Введите дату выдачи книги (дд.мм.гггг): ");
    scanf("%s", loan->issue_date);
    printf("Введите дату возврата книги (дд.мм.гггг): ");
    scanf("%s", loan->return_date);

    reader->borrowed_count++;
}

// Функция для вывода информации о выдаче книги
void print_Loan(Loan loan) {
    //if (loan.book == NULL || loan.reader == NULL || loan.book->title == NULL || loan.reader->fio == NULL) {
    //    printf("Ошибка: некорректные данные о выдаче.\n");
    //    return;
   //}
    printf("Название выданной книги: %s\n", book->title);
    printf("Номер читательского билета: %d\n", reader->card_number);
    printf("Дата выдачи книги: %s\n", loan.issue_date);
    printf("Дата возврата книги: %s\n", loan.return_date);
}

// Функция для вывода всей информации о библиотеке
void print_Library(Library* library) {
    printf("Список авторов:\n");
    for (int i = 0; i < MAX_AUTHORS; i++) {
        if (author[i].fio != NULL) {
            print_Author(author[i]);
        }
    }

    printf("Список книг:\n");
    for (int i = 0; i < library->book_count; i++) {
        if (book[i].title != NULL) {
            print_Book(book[i]);
        }
    }

    printf("Список читателей:\n");
    for (int i = 0; i < library->reader_count; i++) {
        if (reader[i].fio != NULL) {
            print_Reader(reader[i]);
        }
    }

    printf("Список выдач:\n");
    for (int i = 0; i < library->loan_count; i++) {
        if (loan[i].issue_date != NULL) {
            print_Loan(loan[i]);
        }
    }
}

// Функция для возврата книги
void return_Book(Library* library) {
    printf("Выберите книгу для возврата (введите номер): ");
    for (int i = 0; i < library->loan_count; i++) {
        if (library->loans[i] != NULL && library->loans[i]->book != NULL && library->loans[i]->book->title != NULL) {
            printf("%d. %s\n", i + 1, library->loans[i]->book->title);
        }
    }
    int loan_index;
    scanf("%d", &loan_index);
    Loan* loan = library->loans[loan_index - 1];

    // Удаляем книгу из списка взятых книг читателя
    for (int i = 0; i < loan->reader->borrowed_count; i++) {
        if (loan->reader->borrowed_books[i] == loan->book) {
            for (int j = i; j < loan->reader->borrowed_count - 1; j++) {
                loan->reader->borrowed_books[j] = loan->reader->borrowed_books[j + 1];
            }
            loan->reader->borrowed_count--;
            break;
        }
    }

    // Удаляем выдачу из списка выдач
    for (int i = loan_index - 1; i < library->loan_count - 1; i++) {
        library->loans[i] = library->loans[i + 1];
    }
    library->loan_count--;

    printf("Книга успешно возвращена!\n");
}

// Функция для освобождения памяти
void free_Library() {
    for (int i = 0; i < MAX_AUTHORS; i++) {
        if (author[i].fio != NULL) {
            free(author[i].fio);
        }
    }
    for (int i = 0; i < library.book_count; i++) {
        if (library.books[i] != NULL && library.books[i]->title != NULL) {
            free(library.books[i]->title);
        }
    }
    for (int i = 0; i < library.reader_count; i++) {
        if (library.readers[i] != NULL && library.readers[i]->fio != NULL) {
            free(library.readers[i]->fio);
        }
    }
}

int main() {
    SetConsoleCP(1251);          // Устанавливаем кодировку ввода
    SetConsoleOutputCP(1251);    // Устанавливаем кодировку вывода

    printf("БИБЛИОТЕЧНЫЙ УЧЁТ\n");

    int choice;
    do {
        printf("\nМеню:\n");
        printf("1. Добавить автора\n");
        printf("2. Добавить книгу\n");
        printf("3. Добавить читателя\n");
        printf("4. Выдать книгу\n");
        printf("5. Вернуть книгу\n");
        printf("6. Просмотреть все данные\n");
        printf("7. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_Author(&author[library.book_count]); // Добавляем нового автора
            break;
        case 2:
            add_Book(&book[library.book_count]);     // Добавляем новую книгу
            library.books[library.book_count++] = &book[library.book_count - 1]; // Исправлено: добавляем книгу в библиотеку
            break;
        case 3:
            add_Reader(&reader[library.reader_count]); // Добавляем нового читателя
            library.readers[library.reader_count++] = &reader[library.reader_count - 1]; // Исправлено: добавляем читателя в библиотеку
            break;
        case 4:
            add_Loan(&loan[library.loan_count]);       // Добавляем новую выдачу книги
            library.loans[library.loan_count++] = &loan[library.loan_count - 1]; // Исправлено: добавляем выдачу в библиотеку
            break;
        case 5:
            return_Book(&library);                     // Возвращаем книгу
            break;
        case 6:
            print_Library(&library);                   // Выводим все данные о библиотеке
            break;
        case 7:
            printf("Выход из программы.\n");           // Выход из программы
            break;
        default:
            printf("Неверный выбор. Попробуйте снова.\n"); // Обработка неверного выбора
        }
    } while (choice != 7);

    // Освобождаем память перед завершением программы
    free_Library();

    return 0;
}
