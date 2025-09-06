#pragma once
#include "User.h"

/// @class Admin
/// @brief Represents an administrator in the library system.
/// Inherits User functionality and adds an email field.
/// Administrators cannot borrow books.
class Admin : public User
{
public:
	/// @brief Default constructor.
	Admin() = default;

	/// @brief Constructor with parameters.
	/// @param username User name.
	/// @param password Password.
	/// @param registrationDate Registration date.
	/// @param email Email address (optional).
	Admin(const std::string& username, const std::string& password, const Date& registrationDate,
		const std::string& email = "@.");


	/// @brief Outputs information about the administrator.
	/// @brief Format: Admin|username|password|registrationdate|lasttimeaccess|email
	/// @param os Output stream.
	virtual void print(std::ostream& os) const override;

	/// @brief Inputs information about the administrator.
	/// @param is Input stream.
	virtual void read(std::istream& is) override;

	/// @brief Clones the object.
	/// @return Pointer to a new Admin object.
	virtual User* clone() const override {
		return new Admin(*this);
	}

	/// @brief Returns the type of the user.
	/// @return "Admin".
	std::string getType() const override {
		return "Admin";
	}

	// For reader only
	/// @brief Prohibits borrowing books (Admin cannot borrow).
	/// @throw std::invalid_argument
	void AddBorrow(const Borrow& obj) override { throw std::invalid_argument("Is admin! No borrowing to store!\n"); }
	/// @brief Always returns 0 (Admin has no borrows).
	const size_t getCntBorrow() const override { return 0; }
	/// @brief Always returns false (Admin has no borrows).
	bool hasThisBorrowingList(const std::string& title) const override { return false; }
	/// @brief Prohibits removing borrows.
	/// @throw std::invalid_argument
	void removeBorrow(const std::string& title) override { throw std::invalid_argument("Is admin! No borrowing to remove!\n"); }
	/// @brief Always returns false (Admin has no borrows).
	bool hasThisInBorrowingListID(const unsigned ID) const override { return false; }
	/// @brief Always returns false (Admin has no overdue borrows).
	bool hasBorrowingOverdue(const Date& today) const override { return false; }
	/// @brief Always returns false (Admin cannot be inactive regarding borrows).
	bool InactiveUserBorrowing(const Date& today) const override { return false; }
	/// @brief Returns the email address.
	/// @return Constant reference to the email.
	const std::string& getEmail() const { return this->email; }

private:
	/// @brief Checks if the email is valid.
	/// @param email Email to check.
	/// @return true if valid, false otherwise.
	static bool isValidEmail(const std::string& email);
private:
	std::string email{};	///< Email address of the administrator.
};

