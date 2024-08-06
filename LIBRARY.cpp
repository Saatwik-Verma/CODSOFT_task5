#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Book {
    string title;
    string author;
    string isbn;
    bool isAvailable;

    Book(const string &t, const string &a, const string &i)
        : title(t), author(a), isbn(i), isAvailable(true) {}
};

struct Borrower {
    string name;
    vector<pair<string, time_t>> borrowedBooks; // ISBN and borrow timestamp

    Borrower(const string &n) : name(n) {}
};

vector<Book> books;
vector<Borrower> borrowers;

time_t getCurrentTime() {
    return time(0);
}

int calculateFine(time_t borrowTime) {
    time_t currentTime = getCurrentTime();
    double days = difftime(currentTime, borrowTime) / (60 * 60 * 24);
    const int allowedDays = 14;
    const int finePerDay = 2;
    if (days > allowedDays) {
        return (days - allowedDays) * finePerDay;
    }
    return 0;
}

void addBook() {
    string title, author, isbn;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter book author: ";
    getline(cin, author);
    cout << "Enter book ISBN: ";
    getline(cin, isbn);
    books.emplace_back(title, author, isbn);
    cout << "Book added successfully!" << endl;
}

void searchBook() {
    string query;
    cout << "Enter title, author, or ISBN to search: ";
    cin.ignore();
    getline(cin, query);
    for (const auto &book : books) {
        if (book.title == query || book.author == query || book.isbn == query) {
            cout << "Found book: " << book.title << " by " << book.author << " (ISBN: " << book.isbn << ")"
                 << " [" << (book.isAvailable ? "Available" : "Checked out") << "]" << endl;
        }
    }
}

void checkoutBook() {
    string borrowerName, isbn;
    cout << "Enter borrower name: ";
    cin.ignore();
    getline(cin, borrowerName);
    cout << "Enter book ISBN to checkout: ";
    getline(cin, isbn);

    for (auto &book : books) {
        if (book.isbn == isbn && book.isAvailable) {
            book.isAvailable = false;
            bool borrowerExists = false;
            for (auto &borrower : borrowers) {
                if (borrower.name == borrowerName) {
                    borrower.borrowedBooks.push_back({isbn, getCurrentTime()});
                    borrowerExists = true;
                    break;
                }
            }
            if (!borrowerExists) {
                Borrower newBorrower(borrowerName);
                newBorrower.borrowedBooks.push_back({isbn, getCurrentTime()});
                borrowers.push_back(newBorrower);
            }
            cout << "Book checked out successfully!" << endl;
            return;
        }
    }
    cout << "Book not available or not found!" << endl;
}

void returnBook() {
    string borrowerName, isbn;
    cout << "Enter borrower name: ";
    cin.ignore();
    getline(cin, borrowerName);
    cout << "Enter book ISBN to return: ";
    getline(cin, isbn);

    for (auto &borrower : borrowers) {
        if (borrower.name == borrowerName) {
            for (auto it = borrower.borrowedBooks.begin(); it != borrower.borrowedBooks.end(); ++it) {
                if (it->first == isbn) {
                    int fine = calculateFine(it->second);
                    if (fine > 0) {
                        cout << "Book is overdue! Fine: $" << fine << endl;
                    }
                    borrower.borrowedBooks.erase(it);
                    for (auto &book : books) {
                        if (book.isbn == isbn) {
                            book.isAvailable = true;
                            break;
                        }
                    }
                    cout << "Book returned successfully!" << endl;
                    return;
                }
            }
        }
    }
    cout << "Borrower or book not found!" << endl;
}

void menu() {
    while (true) {
        cout << "\nLibrary Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Search Book" << endl;
        cout << "3. Checkout Book" << endl;
        cout << "4. Return Book" << endl;
        cout << "5. Exit" << endl;
        cout << "Choose an option: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                checkoutBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                return;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
}

int main() {
    menu();
    return 0;
}
