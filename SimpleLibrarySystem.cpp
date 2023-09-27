#include <bits/stdc++.h>
using namespace std;

const int BOOKS = 10;
const int USERS = 10;

struct Book {
    int id;
    string name;
    int totalQuantity;
    int totalBorrowed;

    Book() {
        totalQuantity = totalBorrowed = 0;
        id = -1;
        name = "";
    }

    void read() {
        cout << "Enter book info: id & name & total quantity: ";
        cin >> id >> name >> totalQuantity;
        totalBorrowed = 0;
    }

    bool borrow(int userId) {
        if (totalQuantity - totalBorrowed == 0)
            return false;
        ++totalBorrowed;
        return true;
    }

    void returnCopy() {
        assert(totalBorrowed > 0);
        --totalBorrowed;
    }

    bool hasPrefix(string prefix) {
        if (name.size() < prefix.size())
            return false;

        for (int i = 0; i < (int)prefix.size(); ++i) {
            if (prefix[i] != name[i])
                return false;
        }
        return true;
    }

    void print() {
        cout << "id = " << id << " name = " << name << " total quantity "
             << totalQuantity << " total borrowed " << totalBorrowed
             << "\n";
    }
};

bool compareBookByName(Book &a, Book &b) {
    return a.name < b.name;
}

bool compareBookById(Book &a, Book &b) {
    return a.id < b.id;
}

struct User {
    int id;
    string name;
    int borrowedBookIds[BOOKS];
    int len;

    User() {
        name = "";
        len = 0;
        id = -1;
    }

    void read() {
        cout << "Enter user name & national id: ";
        cin >> name >> id;
    }

    void borrow(int bookId) {
        borrowedBookIds[len++] = bookId;
    }

    void returnCopy(int bookId) {
        bool removed = false;
        for (int i = 0; i < len; ++i) {
            if (borrowedBookIds[i] == bookId) {
                for (int j = i + 1; j < len; ++j)
                    borrowedBookIds[j - 1] = borrowedBookIds[j];
                removed = true;
                --len;
                break;
            }
        }
        if (!removed)
            cout << "User " << name << " never borrowed book id " << bookId
                 << "\n";
    }

    bool isBorrowed(int bookId) {
        for (int i = 0; i < len; ++i) {
            if (bookId == borrowedBookIds[i])
                return true;
        }
        return false;
    }

    void print() {
        sort(borrowedBookIds, borrowedBookIds + len);

        cout << "User: " << name << " Id: " << id << " Borrowed books ids: ";
        for (int i = 0; i < len; ++i)
            cout << borrowedBookIds[i] << " ";
        cout << "\n";
    }
};

struct LibrarySystem {
    int totalBooks;
    Book books[BOOKS];
    int totalUsers;
    User users[USERS];

    LibrarySystem() {
        totalBooks = totalUsers = 0;
    }

    void run() {
        while (true) {
            int choice = menu();

            if (choice == 1)
                addBook();
            else if (choice == 2)
                searchBooksByPrefix();
            else if (choice == 3)
                printWhoBorrowedBookByName();
            else if (choice == 4)
                printLibraryById();
            else if (choice == 5)
                printLibraryByName();
            else if (choice == 6)
                addUser();
            else if (choice == 7)
                userBorrowBook();
            else if (choice == 8)
                userReturnBook();
            else if (choice == 9)
                printUsers();
            else
                break;
        }
    }

    int menu() {
        int choice = -1;
        while (choice == -1) {

			cout << "\nLibrary Menu;\n";
			cout << "1) add Book\n";
			cout << "2) search Books By Prefix\n";
			cout << "3) print Who Borrowed Book By Name\n";
			cout << "4) print Library By Id\n";
			cout << "5) print Library By Name\n";
			cout << "6) add User\n";
			cout << "7) user Borrow Book\n";
			cout << "8) user Return Book\n";
			cout << "9) print Users\n";
			cout << "10) Exit\n";
            
            cout << "\nEnter your menu choice [1 - 10]: ";
            cin >> choice;

            if (!(1 <= choice && choice <= 10)) {
                cout << "Invalid choice. Try again\n";
                choice = -1;
            }
        }
        return choice;
    }

    void addBook() {
        books[totalBooks++].read();
    }

    void searchBooksByPrefix() {
        cout << "Enter book name prefix: ";
        string prefix;
        cin >> prefix;

        int count = 0;
        for (int i = 0; i < totalBooks; ++i) {
            if (books[i].hasPrefix(prefix)) {
                cout << books[i].name << "\n";
                ++count;
            }
        }

        if (!count)
            cout << "No books with such prefix\n";
    }

    void addUser() {
        users[totalUsers++].read();
    }

    int findBookIndexByName(string name) {
        for (int i = 0; i < totalBooks; ++i) {
            if (name == books[i].name)
                return i;
        }
        return -1;
    }

    int findUserIndexByName(string name) {
        for (int i = 0; i < totalUsers; ++i) {
            if (name == users[i].name)
                return i;
        }
        return -1;
    }

    bool readUserNameAndBookName(int &userIndex, int &bookIndex, int trials =
    3) {
        string userName;
        string bookName;

        while (trials--) {
            cout << "Enter user name and book name: ";
            cin >> userName >> bookName;

            userIndex = findUserIndexByName(userName);

            if (userIndex == -1) {
                cout << "Invalid user name. Try again\n";
                continue;
            }
            bookIndex = findBookIndexByName(bookName);

            if (bookIndex == -1) {
                cout << "Invalid book name. Try again\n";
                continue;
            }
            return true;
        }
        cout << "You did several trials! Try later.";
        return false;
    }

    void userBorrowBook() {
        int userIndex, bookIndex;

        if (!readUserNameAndBookName(userIndex, bookIndex))
            return;

        int userId = users[userIndex].id;
        int bookId = books[bookIndex].id;

        if (!books[bookIndex].borrow(userId))
            cout << "No more copies available right now\n";
        else
            users[userIndex].borrow(bookId);
    }

    void userReturnBook() {
        int userIndex, bookIndex;

        if (!readUserNameAndBookName(userIndex, bookIndex))
            return;

        int bookId = books[bookIndex].id;
        books[bookIndex].returnCopy();
        users[userIndex].returnCopy(bookId);
    }

    void printLibraryById() {
        sort(books, books + totalBooks, compareBookById);

        cout << "\n";
        for (int i = 0; i < totalBooks; ++i)
            books[i].print();
    }

    void printLibraryByName() {
        sort(books, books + totalBooks, compareBookByName);

        cout << "\n";
        for (int i = 0; i < totalBooks; ++i)
            books[i].print();
    }

    void printUsers() {
        cout << "\n";
        for (int i = 0; i < totalUsers; ++i)
            users[i].print();
    }

    void printWhoBorrowedBookByName() {
        string bookName;
        cout << "Enter book name: ";
        cin >> bookName;

        int bookIndex = findBookIndexByName(bookName);

        if (bookIndex == -1) {
            cout << "Invalid book name.\n";
            return;
        }
        int bookId = books[bookIndex].id;

        if (books[bookIndex].totalBorrowed == 0) {
            cout << "No borrowed copies\n";
            return;
        }

        for (int i = 0; i < totalUsers; ++i) {
            if (users[i].isBorrowed(bookId))
                cout << users[i].name << "\n";
        }
    }
};

int main() {
    LibrarySystem library;

    library.run();

    return 0;
}
