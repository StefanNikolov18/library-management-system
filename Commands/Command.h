#pragma once
#include <string>
#include "../System/System.h"

/// @class Command
/// @brief Class for processing user commands for the system.
/// Allows execution of operations such as login, searching, adding, and removing items and users.
class Command
{
public:
    /// @brief Constructor that initializes the command with the system and the input command.
    /// @param system Reference to the system being used.
    /// @param inputCommand Input string containing the command.
    Command(System& system, const std::string& inputCommand);

    /// @brief Default destructor.
    ~Command() = default;

    /// @brief copy constructor deleted.
    Command(const Command& other) = delete;
    /// @brief operator= deleted.
    Command& operator=(const Command& other) = delete;

    /// @brief Executes the command, performs all preprocessing and calls the necessary function from class System.
    void execute();

private:
    //@brief print commands
    void help() const;


    /// @brief User login.
    void login();
    /// @brief User logout.
    void logout();

    /// @brief Prints all books.
    void booksAll() const;
    /// @brief Prints all newsletters.
    void newslettersAll() const;
    /// @brief Prints all series.
    void seriesAll() const;
    /// @brief Prints all items (books, newsletters, series).
    void ListAll() const;

    /// @brief Searches for information about a library item by ISBN or ISSN.
    /// @param listInfoPref Contains ISBN or ISSN.
    void listInfo(const std::string& listInfoPref) const;

    /// @brief Searches items by given option={title, author, tag}, with sorting (asc, desc) and top N.
    /// @param Prefix Prefix for processing.
    /// @param Type Type of the item.
    void findItem(const std::string& Prefix, const std::string& Type = "") const;

    /// @brief Searches for a user.
    /// @param userFindPrefix Prefix for processing.
    void findUser(const std::string& userFindPrefix) const;

    /// @brief Adds a new book, using ReadDataForBook() and inserts it into the system.
    void BooksAdd() const;
    /// @brief Adds a new newsletter, using ReadDataForPeriodical() and inserts it into the system.
    void NewsletterAdd() const;
    /// @brief Adds a new series, using ReadDataForSeries() and inserts it into the system.
    void SeriesAdd() const;


    /// @brief Adds a new user.
    /// @param addUserPref Prefix containing the user.
    void AddUser(const std::string& addUserPref) const;

    /// @brief Removes a user.
    /// @param removeUserPref Prefix containing the user.
    void RemoveUser(const std::string& removeUserPref) const;

    /// @brief Changes a user's password.
    /// @param userChangePrefix Prefix containing the new password.
    void ChangePassword(const std::string& userChangePrefix) const;

    /// @brief Takes an item by ID.
    /// @param takeIDPrefix Prefix for taking by ID.
    void TakeByID(const std::string& takeIDPrefix) const;
    /// @brief Returns an item by ID.
    /// @param returnIDPrefix Prefix for returning by ID.
    void ReturnID(const std::string& returnIDPrefix) const;

    /// @brief Removes an item from a list.
    /// @param listRemovePrefix Prefix for removing from the list, which is an ID.
    void ListRemove(const std::string& listRemovePrefix) const;
    /// @brief Adds or changes an item in a list.
    /// @param listChangePrefix Prefix for adding or changing, which is an ID.
    void ListChange(const std::string& listChangePrefix) const;

    /// @brief Reads book data from standard input.
    /// @return Pointer to newly created LibraryItem for the book.
    const LibraryItem* ReadDataForBook() const;
    /// @brief Reads newsletter data from standard input.
    /// @return Pointer to newly created LibraryItem for the newsletter.
    const LibraryItem* ReadDataForPeriodical() const;
    /// @brief Reads series data from standard input.
    /// @return Pointer to newly created LibraryItem for the series.
    const LibraryItem* ReadDataForSeries() const;

private:
    System& system; ///< Reference to the system.
    std::string command;  ///< Input command.

    /// @brief Saves changes in books to the file with fileNameBook.
    void checkPointBooks() const;
    /// @brief Saves changes in users to the file with fileNameUsers.
    void checkPointUsers() const;
};

/// @brief Checks if the command starts with a given prefix.
/// @param command Input string.
/// @param prefix Searched prefix.
/// @return true if the command starts with the prefix, false otherwise.
inline bool startWith(const std::string& command, const std::string& prefix) {
    return command.compare(0, prefix.size(), prefix) == 0;
}