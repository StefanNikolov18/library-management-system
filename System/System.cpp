#include "System.h"
#include "../Users/Admin.h"
#include "../Users/Reader.h"
#include <iostream>
#include <exception>
#include <vector>
#include <algorithm>

//Constructor loading once from files
System::System(std::ifstream& isLI, std::ifstream& isUsers,
    const std::string& fileNameBooks, const std::string& fileNameUsers)
    : ifs(isLI), fileNameBooks(fileNameBooks), fileNameUsers(fileNameUsers)
{
    if (!isLI.is_open())
        throw std::invalid_argument("Cannot open Library Items File!");

    if (!isUsers.is_open())
        throw std::invalid_argument("Cannot open Users File!");

    try {
        loadCountBooks(); //load only MAXSTORING count books

        while (true) { //loading all users from file
            User* user = User::UserFactory(isUsers);
            if (!user) break;
            users.push_back(user);
        }
    }
    catch (...) {
        FreeBooks();
        FreeUsers();
        throw;
    }

    std::cout << "System is load!\n";

    //if nothing in fileNameBooks or fileNameUsers
    if (books.empty() || users.empty()) {
        username = "admin";
        Date today = getTodayDate();
        Admin admin("admin", "i<3c++", today, "@.");
        if(!users.size() || !isInUsers(username))
            users.push_back(admin.clone());

        std::cout << "No data found. Default administrator account created:\n"
            << "Username: admin\n"
            << "Password: i<3c++\n";
    }
}

System::~System() noexcept {
    FreeBooks();
    FreeUsers();
}

//Savings
void System::saveToFiles() {
    LoadAllLibraryItemsInBooks();
    saveToFileBooks();
    saveToFileUsers();
}
void System::saveToFileBooks() const {
    std::ofstream fileBooks(fileNameBooks, std::ios::out);
    if (!fileBooks.is_open())
        throw std::invalid_argument("File Books cannot be opened!");

    for (auto* book : books) {
        book->print(fileBooks);
        fileBooks << '\n';
    }

    if (fileBooks.fail())
        throw std::invalid_argument("Failed to write to Books.txt");

    fileBooks.close();
}
void System::saveToFileUsers() const {
    std::ofstream fileUsers(fileNameUsers, std::ios::out);
    if (!fileUsers.is_open())
        throw std::invalid_argument("File Users cannot be opened!");

    for (auto* user : users) {
        user->print(fileUsers);
        fileUsers << '\n';
    }

    if (fileUsers.fail())
        throw std::invalid_argument("Failed to write to Users.txt");

    fileUsers.close();
}

//Login
bool System::isInUserAndThisPassword(const std::string& username, const std::string& password) {
    for (auto* user : users) {
        if (user->getUsername() == username)
            return user->getPassword() == password;
    }
    return false;
}
void System::loginUser(const std::string username)
{
    this->username = username;

    Date today = getTodayDate();

    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getUsername() == this->username) {
            users[i]->setLastAccessDate(today);
        }
    }
}

//is Admin helpingFUnction
bool System::isAdmin() const
{
   //first i make sure username is not empty in command
    if (username.empty()) {
        throw std::invalid_argument("Problem in isAdmin!");
    }

    for (size_t i = 0; i < users.size();++i) {
        if (users.at(i)->getUsername() == this->username) { //where is in users
            return users.at(i)->getType() == "Admin";
        }
    }

    return false;
}

//List item of type = {book,newsletter,series}
//Command: books all, newsletters all, series all
void System::ListLibraryItems(const std::string& type) {

    std::vector<LibraryItem*> list;
    resetBooks(); //is needed because all its printed double

    try {
        while (true) {
            for (auto* book : books) {
                if (book->getType() == type)
                    list.push_back(book->clone()); //exception
            }

            if (!nextBooks())//exception
                break;
        }

    }
    catch (...) {
        for (auto* item : list)
            delete item;
        throw;
    }
    
    //resetBooks
    resetBooks();

    //no items of this type
    if (list.empty()) {
        std::cout << "No items of type " << type << " found.\n";
        return;
    }

    size_t i = 0, pageSize = PAGES;
    char choice = 'N';
    do {
        while (i < pageSize && i < list.size()) {
            list[i]->printFiltered();
            std::cout << "-----------------------------------------\n";
            ++i;
        }

        if (i >= list.size()) {
            std::cout << "End of list.\n";
            break;
        }

        std::cout << "Do you want to be shown more (Y/N) : ";
        std::cin >> choice;
        std::cin.ignore();
        pageSize += PAGES;

    } while (choice == 'Y' || choice == 'y');

    for (auto* item : list)
        delete item;
}

//List all Items
//Command: list all
void System::ListAllLibraryItems() {
   
    std::vector<LibraryItem*> list;
    try {
        reset();
        loadAllFileBooks(list, ifs); //want to load all in list
    }
    catch (...) {
        for (auto* item : list)
            delete item;
        throw;
    }
    reset();

    size_t i = 0, pages = PAGES;
    char choice = 'Y';
    do {
        while (i < pages && i < list.size()) {
            list[i]->printFiltered();
            std::cout << "-----------------------------------------\n";
            ++i;
        }

        if (i >= list.size()) {
            std::cout << "End of list.\n";
            break;
        }

        std::cout << "Do you want to be shown more (Y/N) : ";
        std::cin >> choice;
        std::cin.ignore();
        pages += PAGES;

    } while (choice == 'Y' || choice == 'y');

    for (auto* item : list)
        delete item;

    
}

// Command: list info <ISBN?ISSN>
void System::ListLibraryItemInfo(const std::string& isbnissn)
{

    while (true) {
        for (size_t i = 0; i < books.size();++i) { //loking only count books

            if (books.at(i)->getIsbnIssn() == isbnissn) {//if found
                books.at(i)->print();
                std::cout << '\n';
                resetBooks(); //reset
                return; //found
            }
        }

        if (!nextBooks()) { //if not in first loading the next
            std::cerr << "No LibraryItem found with ISBN/ISSN: " << isbnissn << "\n"; //not found
            resetBooks(); //restore
            break;
        }
    }
}


//Find by option LibraryItem
//Command: type find <option> <option_string> [sort <key> [asc|desc] ] [top <val>]
void System::findItemByOption(const std::string& Type, const std::string& option,
    const std::string& option_string, bool hasSort, const std::string& sortKey, bool asc, int topCount)
{
    std::vector<LibraryItem*> list; //for storing by type
    resetBooks();
    try {
        while (true) {
            for (auto* book : books) {
                if (book->getType() == Type) //for type find
                    list.push_back(book->clone());
                else if(Type == "") //for list find command
                    list.push_back(book->clone());
            }

            if (!nextBooks())
                break;
        }

        
    }
    catch (...) {
        for (auto* item : list)
            delete item;
    
        throw;
    }

    //resetBooks
    resetBooks();

    if (list.empty()) {
        std::cout << "No items of type " << Type << " found.\n";
        return;
    }

    // Filter by key (title, author, tag)
    std::vector<LibraryItem*> filtered;
    try {
        for (size_t i = 0; i < list.size(); ++i) {
            if (option == "title" && list[i]->getTitle() == option_string) {
                filtered.push_back(list[i]->clone());
            }
            else if (option == "author" && list[i]->getAuthor() == option_string) {
                filtered.push_back(list[i]->clone());
            }
            else if (option == "tag" && list[i]->getDescription().find(option_string) != std::string::npos) { 
                filtered.push_back(list[i]->clone());
            }
        }
    }
    catch (...) {
        for (auto* item : list)
            delete item;
        for (auto* item : filtered) //filtered
            delete item;
        throw;
    }

    for (auto* item : list) //free list
        delete item;
    
    try {
        // Сортиране
        if (hasSort) {
            std::sort(filtered.begin(), filtered.end(), [&](LibraryItem* a, LibraryItem* b) {
                if (sortKey == "title")
                    return asc ? a->getTitle() < b->getTitle() : a->getTitle() > b->getTitle();
                if (sortKey == "year")
                    return asc ? a->getYear() < b->getYear() : a->getYear() > b->getYear();
                if (sortKey == "rating")
                    return asc ? a->getRating() < b->getRating() : a->getRating() > b->getRating();
                return true;
            });
        }
    }
    catch (...) {
       
        for (auto* item : filtered) //filtered
            delete item;
        throw;
    }
    
    
    if (topCount > 0 && topCount < (int)filtered.size()) {
        for (int i = topCount+1;i < filtered.size();++i)
            delete filtered[i];
        filtered.resize(topCount);
    }

    if (filtered.size() == 0) {
        std::cerr << "Not found item, sorry!\n";
    }

    
    for (auto* item : filtered) {
        item->printFiltered();
    }

    for (auto* item : filtered) //filtered
        delete item;
   
}

//Find User by option
//Command: user find
void System::findUserByState(const std::string stateString)
{
    //date today
    Date today = getTodayDate();
   
    for (size_t i = 0;i < users.size();++i) {
        if (stateString == "overdue" && users[i]->getType() == "Reader" && users[i]->getCntBorrow() != 0) {
            if (users[i]->hasBorrowingOverdue(today)) {
                users[i]->printFiltered();
                std::cout << '\n';
            }
        }
        else if (stateString == "reader" && users[i]->getType() == "Reader" && users[i]->getCntBorrow() >= 5) {
            users[i]->printFiltered();
            std::cout << '\n';
        }
        else if (stateString == "inactive" && users[i]->getType() == "Reader") {
            if (users[i]->InactiveUserBorrowing(today)) {
                users[i]->printFiltered();
                std::cout << '\n';
            }
        }
    }
}
//Command: user find
void System::findUserByName(const std::string& option_string)
{
    for (size_t i = 0;i < users.size();++i) {
        if (users[i]->getUsername() == option_string) {
            users[i]->printFiltered();
            std::cout << '\n';
            return;
        }
    }
}
//Command: user find
void System::findUsersByID(const std::string& option_string)
{
    for (size_t i = 0;i < users.size();++i) {
        if (users[i]->getType() == "Reader" && users[i]->getCntBorrow() != 0) { //Type Reader and has borrowing
            if (users[i]->hasThisInBorrowingListID(atoi(option_string.c_str()))) {
                users[i]->printFiltered();
                std::cout << '\n';
            }
        }
    }
}

//books add


//Command: list remove ID
void System::removeListByID(const std::string& ID)
{
    //Load all libraryItems
    reset();
    FreeBooks();
    LoadAllLibraryItemsInBooks();

    unsigned index = atoi(ID.c_str());

    if (index >= books.size()) {
        std::cerr << "Invalid ID!\n";
        return;
    }

    const std::string title = books[index]->getTitle();

    delete books[index];
    books.erase(books.begin() + index); //deleting


    saveToFileBooks(); //save book
    resetBooks();

    //looking for readers
    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getType() == "Reader" && users[i]->getCntBorrow()) {
            users[i]->removeBorrow(title);
        }
    }
    std::cout << "Removing was succesful with ID: " << index << '\n';

}

//Command: list change ID
void System::changeListByID(const std::string& ID)
{
    //Load all libraryItems
    reset();
    FreeBooks();
    LoadAllLibraryItemsInBooks();

    unsigned id = atoi(ID.c_str());
    if (id >= books.size()) {
        std::cerr << "Invalid ID!\n";
        return;
    }

    std::cout << "Type:" << books[id]->getType() << '\n';

    bool valid = false;
    while (!valid) {
        std::cout << "Enter what you wanna change: \n"
            << "Title - 1\nGenre-2\nPublisher-3\nRating-4\nDescription-5\n"
            << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        std::string temp;
        int x;
        double y;
        switch (choice) {
        case 1:
            std::cout << "Enter new Title: ";
            std::getline(std::cin, temp);
            books[id]->setTitle(temp);
            std::cout << "The new title was changed\n";
            valid = true;
            break;
        case 2:
            std::cout << "Enter new genre: ";
            std::cin >> x;
            books[id]->setGenre((GENRE)x);
            std::cout << "New genre was set\n";
            valid = true;
            break;
        case 3:
            std::cout << "Enter new Publisher: ";
            std::getline(std::cin, temp);
            books[id]->setPublisher(temp);
            std::cout << "The new publisher was changed\n";
            valid = true;
            break;
        case 4:
            std::cout << "Enter new rating(0.0 to 9.9): ";
            std::cin >> y;
            books[id]->setRating(y);
            if (y < 0.0 || y > 9.99)
                std::cout << "Invalid rating set to 0.0\n";
            std::cout << "New rating was set\n";
            valid = true;
            break;
        case 5:
            std::cout << "Enter new Description: ";
            std::getline(std::cin, temp);
            books[id]->setDescription(temp);
            std::cout << "The new description was changed\n";
            valid = true;
            break;

        default:
            std::cerr << "Invalid change type again!\n";
        }

    }

    saveToFileBooks(); //save book
    resetBooks();


}

//addUser
//Command: add user 
void System::addUser(std::string& userSpecs)
{
    //splits words in the string and store it in vector
    std::vector<std::string> tokens;
    splitsWords(tokens, userSpecs);

    Date today = getTodayDate();

    if (isInUsers(tokens[0])) {
        std::cerr << "Sorry, this username was taken!\n";
        return;
    }

    if (tokens.size() == 3) {
        Admin newAdmin(tokens[0], tokens[1], today, "@.");
        User* user = newAdmin.clone();
        AddInUsers(user);
        delete user;
        std::cout << "User Admin was added successfully!\n";

    }
    else if (tokens.size() == 2) {
        Reader newReader(tokens[0], tokens[1], today, {});
        User* user = newReader.clone();
        AddInUsers(user);
        delete user;
        std::cout << "User reader was added successfully!\n";
    }
    else {
        std::cerr << "Invalid user format.\n";
    }

}


void System::splitsWords(std::vector<std::string>& tokens,std::string& userSpecs) {

    const char* ptr = userSpecs.c_str();
    std::string temp;

    while (*ptr) {
        if (*ptr == ' ') {
            if (!temp.empty()) {
                tokens.push_back(temp);
                temp.clear();
            }
        }
        else {
            temp += *ptr;
        }
        ++ptr;
    }

    if (!temp.empty()) {
        tokens.push_back(temp);
    }

    
}

bool System::isInUsers(const std::string& username) const
{
    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getUsername() == username)
            return true;
    }

    return false;
}


//remove user function
//COmmand: User remove
void System::removeUser(const std::string& username)
{
    for (size_t i = 0; i < users.size();++i) {
        if (this->users.at(i)->getUsername() == username) {

            
            if (this->users.at(i)->getType() == "Admin") {
                std::cerr << "Admin cannot be removed!\n";
                return;
            }
            else { //not admin
               
                if (users[i]->getCntBorrow()) { //if has borrowings
                    std::cout << "The user has borrowings he didn't returned!\n";
                    std::cout << "Do you want to be removed(Y/N): ";
                    char choice = 'n';
                    std::cin >> choice;

                    if (choice == 'y' || choice == 'Y') {
                        delete users.at(i); //delete
                        users.erase(users.begin() + i); //remove
                        std::cout << "The user was removed successfully!" << std::endl;
                        return;
                    }
                    else { //if choice was NO
                        std::cout << "The removing operation was canceled!" << std::endl;
                        return;
                    }
                }
          
                delete users[i]; //delete
                users.erase(users.begin() + i); //remove
                std::cout << "The user was removed successfully!" << std::endl;
                return;

            }
        }
    }

    std::cerr << "Invalid username. Not found in System!\n";
}

//Command: User change
void System::changePassword(const std::string& usernameH, const std::string& password)
{
    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getUsername() == usernameH) {
            users[i]->changePass(password);
            std::cout << "The password was changed!\n";
            return;
        }
    }

    std::cerr << "No user found with this username in users!\n";
}


#define BorowingLimit 14
//Command: take ID
void System::borrowID(const std::string& ID)
{
    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getUsername() == username) {
            
            LibraryItem* item = BorrowbyID(atoi(ID.c_str()));
            if (item == nullptr) {
                std::cerr << "No book with this ID\n";
                return;
            }
            
            Date today = getTodayDate();
            Borrow b(item, today, today.addDays(BorowingLimit));
            try {
                users[i]->AddBorrow(b);
                std::cout << "Item was succesfully borrowed!\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Error while borrowing: " << e.what() << "\n";
            }
           
            

            return;
        }
    }
}
LibraryItem* System::BorrowbyID(const unsigned ID)
{
    reset();
    FreeBooks();
    LoadAllLibraryItemsInBooks();
    unsigned index = ID; //index for id
    if (index >= books.size()) {
        std::cerr << "Invalid ID!\n";
        return nullptr;
    }

    if (books[index]->getUniqueID() == ID) {
        if (books[index]->getCopiesCNT()) {
            books[index]->setCopiesCNT(books[index]->getCopiesCNT() - 1); //remove one copy from library
            saveToFileBooks(); //save book
            LibraryItem* item = books[index]->clone();
            resetBooks();
            return item;
        }
        else {
            resetBooks();
            std::cerr << "No more copies in Library. Sorry!\n";
            return nullptr;
        }
    }



    resetBooks();
    return nullptr;
}

//Command: return ID
void System::returnID(const std::string& ID)
{
    reset();
    FreeBooks();
    LoadAllLibraryItemsInBooks();

    unsigned id = atoi(ID.c_str());
    if (id >= books.size()) {
        std::cerr << "Invalid ID!";
        return;
    }
    for (size_t i = 0; i < users.size();++i) {
        if (users[i]->getUsername() == username) {
            if (!users[i]->hasThisBorrowingList(books[id]->getTitle())) {
                std::cerr << "No library Item in your inventory!\n";
                resetBooks();
                return;
            }
            else {
                books[id]->setCopiesCNT(books[id]->getCopiesCNT() + 1);
                try {
                    users[i]->removeBorrow(books[id]->getTitle());
                }
                catch (const std::exception& e) {
                    std::cerr << "Borrow failed: " << e.what() << "\n";
                }
               
                saveToFileBooks(); //save book
                resetBooks();
            }
        }
       
    }
    resetBooks();
}




//For free Momory from vector

void System::FreeBooks() {
    for (auto* book : this->books)
        delete book;
    books.clear();
}

void System::FreeUsers() {
    for (auto* user : this->users)
        delete user;
    users.clear();
}

//Helpers VVV


void System::LoadAllLibraryItemsInBooks() //free clear and render
{
    FreeBooks(); //free old memory
    books.clear(); //clearing vector
    reset(); //reset
    loadAllFileBooks(this->books, this->ifs); //Render all from Files Book
}

void System::AddInBooks(const LibraryItem* obj)
{
    if (obj == nullptr)
        throw std::invalid_argument("Obj LibraryItem* is nullptr!");

    books.push_back(obj->clone());
}

void System::AddInUsers(const User* obj)
{
    if (obj == nullptr)
        throw std::invalid_argument("Obj User* is nullptr!");

    users.push_back(obj->clone());
}

void System::reset() const {
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
    LibraryItem::NextID = 0;
}


//Use reset, Free, loadCount
void System::resetBooks() {
    reset();
    FreeBooks();
    loadCountBooks();
}

void System::loadCountBooks(size_t count) {
    LibraryItem* item = nullptr;
    while (count--) {
        item = LibraryItem::LibraryFactory(ifs);
        if (!item) break;
        books.push_back(item);
    }
}

bool System::nextBooks(size_t count) {
    if (ifs.eof())
        return false;

    FreeBooks();
    loadCountBooks(count);
    return true;
}

void System::loadAllFileBooks(std::vector<LibraryItem*>& list, std::ifstream& ifs) {

    LibraryItem* item = nullptr;
    while (true) {
        item = LibraryItem::LibraryFactory(ifs);
        if (!item)
            break;
        list.push_back(item);
    }
}