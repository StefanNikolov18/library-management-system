#pragma once
#include <string>
#include "../Borrow/Borrow.h"
#include "../Date/Date.h"



/// @class User
/// @brief Abstract base class representing a user in the system.
class User
{
public:
	/// @brief Default constructor.
	User() = default;

	/// @brief Constructor with parameters.
	/// @param username User name.
	/// @param password Password.
	/// @param registrationDate Registration date.
	User(const std::string& username, const std::string& password, const Date& registrationDate);

	/// @brief Constructor with parameters including last access.
	/// @param username User name.
	/// @param password Password.
	/// @param registrationDate Registration date.
	/// @param lastAccess Last access date.
	User(const std::string& username, const std::string& password, const Date& registrationDate, const Date& lastAccess);

	/// @brief Virtual destructor.
	virtual ~User() noexcept = default;


	/// @brief Outputs the user information.
	/// @param os Output stream.
	virtual void print(std::ostream& os) const = 0;

	/// @brief Reads user information from a stream.
	/// @param is Input stream.
	virtual void read(std::istream& is) = 0;

	/// @brief Creates a copy of the object.
	/// @return Pointer to a new user.
	virtual User* clone() const = 0;

	/// @brief Returns the type of the user (e.g., Reader/Admin).
	/// @return Type as a string.
	virtual std::string getType() const = 0;

	/// @brief Outputs filtered information (not overridden).
	virtual void printFiltered() const;

	/// @brief Changes the user's password.
	/// @param newPass New password.
	virtual void changePass(const std::string& newPass) { this->password = newPass; }

	//virtual for Reader
	/// @brief Adds a new borrow record.
	/// @param obj Object of type Borrow.
	virtual void AddBorrow(const Borrow& obj) = 0;
	/// @brief Returns the count of current borrows.
	virtual const size_t getCntBorrow() const = 0;
	/// @brief Checks if a title is in the borrowing list.
	/// @param title Title to search for.
	virtual  bool hasThisBorrowingList(const std::string& title) const = 0;
	/// @brief Checks if a borrowing with the given ID exists.
	/// @param ID Unique identifier.
	virtual bool hasThisInBorrowingListID(const unsigned ID) const = 0;
	/// @brief Removes a borrow by title.
	/// @param title Title of the item.
	virtual void removeBorrow(const std::string& title) = 0;
	/// @brief Checks if the user has overdue borrows.
	/// @param today Today's date.
	virtual bool hasBorrowingOverdue(const Date& today) const = 0;
	/// @brief Checks if the user is inactive based on their borrows.
	/// @param today Today's date.
	virtual bool InactiveUserBorrowing(const Date& today) const = 0;

	//getters
	const std::string& getUsername() const { return this->username; }
	const std::string& getPassword() const { return this->password; }
	const Date& getRegistrationDate() const { return this->registrationDate; }
	const Date& getLastAccess() const { return this->lastAccess; }

	//setters
	void setUsername(const std::string& newUsername) { this->username = newUsername; }
	void setPassword(const std::string& newPassword) { this->password = newPassword; }
	void setRegistrationDate(const Date& newRegisterDate) { this->registrationDate = newRegisterDate; }
	void setLastAccessDate(const Date& newLastAccessDate) { this->lastAccess = newLastAccessDate; }

	

	/// @brief Creates a new user from a stream.
	/// @param is Input file stream.
	/// @return Pointer to the new user.
	static User* UserFactory(std::ifstream& is);


private:
	std::string username{};         ///< User's username.
	std::string password{};         ///< User's password.
	Date registrationDate{};        ///< User's registration date.
	Date lastAccess{};              ///< Date of last system access.

	
};

