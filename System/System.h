#pragma once
#include <vector>

#include "../Library/LibraryItem.h"
#include "../Users/User.h"
#include "../Users/Reader.h"

#include "../Options/Options.h"

#include <fstream>


/// @class System
/// @brief The main class that manages the library system � users, books, and command processing.
/// @brief Contains collections of users and library items.
class System
{
	friend class Command;
public:
	/// @brief Constructor that loads users and books from files.
	/// @param isLI Input file stream for LibraryItems.
	/// @param isUsers Input file stream for users.
	/// @param fileNameBooks Name of the books file.
	/// @param fileNameUsers Name of the users file.
	System(std::ifstream& isLI, std::ifstream& isUsers,
		const std::string& fileNameBooks, const std::string& fileNameUsers);

	/// @brief Destructor � releases dynamically allocated memory in the collections.
	~System() noexcept;

	/// @brief Deleted copy constructor.
	System(const System& other) = delete;

	/// @brief Deleted copy assignment operator.
	System& operator=(const System& other) = delete;

	/// @brief Saves all changes to the files (used for exit command).
	void saveToFiles();

	/// @brief Saves books to a file named fileNameBooks.
	void saveToFileBooks() const;
	/// @brief Saves users to a file named fileNameUsers.
	void saveToFileUsers() const;

	
	/// @brief Checks if there is a user with the given username and password.
	/// @brief For command "login".
	/// @param username username of the user
	/// @param password password for user
	bool isInUserAndThisPassword(const std::string& username, const std::string& password);

	/// @brief Login user by username and change date of lastAccess by today.
	void loginUser(const std::string username);

	/// @brief Checks if the currently logged-in user is an administrator.
	/// @brief Mainly checks for most command the want administrator rights.
	bool isAdmin() const;

	/// @brief Prints library items by type.
	/// @brief For commands "books all", "newsletters all", "series all".
	void ListLibraryItems(const std::string& type);

	/// @brief Prints all library items.
	/// @brief For command "list all"
	void ListAllLibraryItems();


	/// @brief Shows details for an item by ISBN/ISSN.
	// @brief For command "list info ..."
	void ListLibraryItemInfo(const std::string& isbnissn);

	/// @brief Searches for library items by type, criteria, and sorting options.
	/// @brief For command "books find ...", "series find ..." , "newsletters find ...", "list find ..."
	void findItemByOption(
		const std::string& Type,             // "Book", "Periodical", "Series", if "" - from all
		const std::string& option,           // "title", "author", "tag"
		const std::string& option_string,    // string to search
		bool hasSort,                        // whether sorting is enabled
		const std::string& sortKey,          // sorting key
		bool asc,                            // true = ascending, false = descending
		int topCount                         // -1 if no limit, else limit count 
	);



	/// @brief Searches users by status (reader/overdue/inactive).
	/// @brief For command "user find state ..."
	void findUserByState(const std::string stateString);
	/// @brief Searches users by name.
	/// @brief For command "user find name ..."
	void findUserByName(const std::string& option_string);
	/// @brief Searches users by ID.
	/// @brief For command "user find ID ..."
	void findUsersByID(const std::string& option_string);

	/// @brief Adds a new item to the library.
	/// @brief For command "books add", "series add", "newsletters add".
	void AddInBooks(const LibraryItem* obj);


	/// @brief Removes an item from the list by ID.
	/// @brief For command "list remove <ID>".
	void removeListByID(const std::string& ID);

	/// @brief Modifies an item in the list by ID.
	/// @brief For command "list change <ID>".
	void changeListByID(const std::string& ID);


	/// @brief Adds a new user.
	/// @brief Using AddInUser().
	/// @brief For command "user add".
	void addUser(std::string& userSpecs);
	/// @brief Adds a new user.
	/// @brief For command "user add".
	void AddInUsers(const User* obj);

	/// @brief Removes a user, with option to remove if they have borrowed books or not.
	/// @brief For command "user remove".
	void removeUser(const std::string& username);


	/// @brief Changes the password for a given user.
	/// @brief For command "user change ...".
	void changePassword(const std::string& username, const std::string& password);

	/// @brief Helper function for borrowID().
	/// @brief Borrows from the library.
	/// @return Returns a pointer to the item by ID (for borrowing).
	LibraryItem* BorrowbyID(const unsigned ID);

	/// @brief Borrows an item by ID.
	/// @brief For command "take <ID>".
	/// @param ID string.
	void borrowID(const std::string& ID);

	/// @brief Returns an item by ID.
	/// @brief For command "return <ID>".
	/// @param ID string.
	void returnID(const std::string& ID);

	
private:
	
	/// @brief Loads all library items into the vector books.
	void LoadAllLibraryItemsInBooks();

	/// @brief Frees memory allocated for books in the books vector.
	void FreeBooks();
	/// @brief Frees memory allocated for users in the users vector.
	void FreeUsers();

	/// @brief Resets the input stream.
	void reset() const;

	/// @brief Loads a specific number of books from file.
	void loadCountBooks(size_t count = option::libraryConfig::MAXSTORING);

	/// @brief Resets the display to show the first MAXSTORING books.
	void resetBooks();

	/// @brief Loads the next portion of books.
	/// @param count Number of books to load, default is MAXSTORING.
	bool nextBooks(size_t count = option::libraryConfig::MAXSTORING);

	/// @brief Loads all books from a file into a vector.
	/// @param list Vector to store all library items from the file.
	/// @param ifs Input file stream to read books from.
	static void loadAllFileBooks(std::vector<LibraryItem*>& list, std::ifstream& ifs);

	/// @brief Splits input string into words by spaces.
	static void splitsWords(std::vector<std::string>& tokens, std::string& userSpecs);

	/// @brief Check if username is in users or not.Helper fo addUser().
	/// @param username Username of new user.
	/// @return true if is in users,false otherwise.
	bool isInUsers(const std::string& username) const;

	std::vector<LibraryItem*> books{};     ///< Vector of pointers to library items (books, periodicals, series).
	std::vector<User*> users{};            ///< Vector of pointers to all users in the system.

	std::string username{};                ///< Username of the currently logged-in user. Empty string if none logged in.

	std::ifstream& ifs;                    ///< Reference to input stream used to load books from file.

	std::string fileNameBooks;             ///< Name of the file where library items are stored.
	std::string fileNameUsers;             ///< Name of the file where users are stored.
};