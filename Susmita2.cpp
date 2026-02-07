#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool issued;

public:
    Book() : id(0), title(""), author(""), issued(false) {}

    Book(int i, string t, string a) {
        id = i;
        title = t;
        author = a;
        issued = false;
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }

    void issueBook() { issued = true; }
    void returnBook() { issued = false; }

    void display() const {
        cout << left << setw(5) << id
             << setw(25) << title
             << setw(20) << author
             << setw(10) << (issued ? "Issued" : "Available") << endl;
    }

    string serialize() const {
        return to_string(id) + "|" + title + "|" + author + "|" + (issued ? "1" : "0");
    }

    static Book deserialize(const string& line) {
        int id;
        string title, author, issuedStr;
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);

        id = stoi(line.substr(0, p1));
        title = line.substr(p1 + 1, p2 - p1 - 1);
        author = line.substr(p2 + 1, p3 - p2 - 1);
        issuedStr = line.substr(p3 + 1);

        Book b(id, title, author);
        if (issuedStr == "1") b.issueBook();
        return b;
    }
};

class Library {
private:
    vector<Book> books;
    const string fileName = "library.txt";

public:
    Library() {
        loadFromFile();
    }

    void addBook() {
        int id;
        string title, author;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        books.push_back(Book(id, title, author));
        saveToFile();
        cout << "Book added successfully.\n";
    }

    void displayBooks() const {
        if (books.empty()) {
            cout << "No books available.\n";
            return;
        }

        cout << left << setw(5) << "ID"
             << setw(25) << "Title"
             << setw(20) << "Author"
             << setw(10) << "Status" << endl;

        cout << string(60, '-') << endl;

        for (const auto& b : books) {
            b.display();
        }
    }

    void issueBook() {
        int id;
        cout << "Enter Book ID to issue: ";
        cin >> id;

        for (auto& b : books) {
            if (b.getId() == id) {
                if (!b.isIssued()) {
                    b.issueBook();
                    saveToFile();
                    cout << "Book issued successfully.\n";
                } else {
                    cout << "Book already issued.\n";
                }
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        for (auto& b : books) {
            if (b.getId() == id) {
                if (b.isIssued()) {
                    b.returnBook();
                    saveToFile();
                    cout << "Book returned successfully.\n";
                } else {
                    cout << "This book was not issued.\n";
                }
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void searchBook() const {
        int id;
        cout << "Enter Book ID to search: ";
        cin >> id;

        for (const auto& b : books) {
            if (b.getId() == id) {
                cout << "Book Found:\n";
                cout << left << setw(5) << "ID"
                     << setw(25) << "Title"
                     << setw(20) << "Author"
                     << setw(10) << "Status" << endl;
                cout << string(60, '-') << endl;
                b.display();
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void saveToFile() const {
        ofstream out(fileName);
        for (const auto& b : books) {
            out << b.serialize() << endl;
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in(fileName);
        if (!in) return;

        string line;
        while (getline(in, line)) {
            if (!line.empty()) {
                books.push_back(Book::deserialize(line));
            }
        }
        in.close();
    }
};

void menu() {
    cout << "\n===== Library Management System =====\n";
    cout << "1. Add Book\n";
    cout << "2. Display Books\n";
    cout << "3. Issue Book\n";
    cout << "4. Return Book\n";
    cout << "5. Search Book\n";
    cout << "6. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    Library lib;
    int choice;

    do {
        menu();
        cin >> choice;

        switch (choice) {
        case 1:
            lib.addBook();
            break;
        case 2:
            lib.displayBooks();
            break;
        case 3:
            lib.issueBook();
            break;
        case 4:
            lib.returnBook();
            break;
        case 5:
            lib.searchBook();
            break;
        case 6:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}