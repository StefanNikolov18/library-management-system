#pragma once
#include "User.h"
#include "../Date/Date.h"
#include "../Library/LibraryItem.h"
#include "../Borrow/Borrow.h"

#include <vector>
#include <iostream>
#include <fstream>

/// @class Reader
/// @brief Represents a user - reader with the ability to borrow library items.
class Reader : public User
{
public:
	/// @brief Default constructor.
	Reader() = default;

	/// @brief Constructor with parameters.
	/// @param username User name.
	/// @param password Password.
	/// @param registrationDate Registration date.
	/// @param borrowingItems List of borrowings.
	Reader(const std::string& username, const std::string& password, const Date& registrationDate,
		const std::vector<Borrow>& borrowingItems);


	/// @brief Outputs information about the reader.
	/// @brief Format: Reader|username|password|registrationdate|lasttimeaccess|cntBorowing|
	/// BorrowingItemsPrintformat...|borrowdate|returningdate|\n|BorrowingItemsPrintformat...
	/// @param os Output stream.
	virtual void print(std::ostream& os) const override;

	/// @brief Reads information about the reader from input stream.
	/// @param is Input stream.
	virtual void read(std::istream& is) override;

	/// @brief Clones the object.
	/// @return Pointer to a copy of the object.
	virtual User* clone() const override { // clone
		return new Reader(*this);
	}

	/// @brief Returns the type of the user.
	/// @return String "Reader".
	std::string getType() const override {
		return "Reader";
	}

	// For system vvv

	/// @brief Adds a borrow to the borrowing list.
	/// @param obj Borrow object to add.
	void AddBorrow(const Borrow& obj) override { borrowingItems.push_back(obj); }

	/// @brief Checks if the reader has a borrow with the given title.
	/// @param title Title of the library item.
	/// @return true if such a borrow exists, false otherwise.
	bool hasThisBorrowingList(const std::string& title) const override;

	/// @brief Removes a borrow with the given title from the list.
	/// @param title Title of the library item.
	void removeBorrow(const std::string& title) override;

	/// @brief Checks if a borrow with the given ID exists in the list.
	/// @param ID Identifier of the library item.
	/// @return true if a borrow with this ID exists, false otherwise.
	bool hasThisInBorrowingListID(const unsigned ID) const override;

	/// @brief Checks if the reader has overdue borrows by a given date.
	/// @param today Current date.
	/// @return true if there are overdue borrows, false otherwise.
	bool hasBorrowingOverdue(const Date& today) const override;

	/// @brief Checks if the user is inactive regarding borrows by a given date.
	/// @param today Current date.
	/// @return true if inactive, false otherwise.
	bool InactiveUserBorrowing(const Date& today) const override;

	/// @brief Returns the number of borrows.
	/// @return Size of the borrowing list.
	const size_t getCntBorrow() const override { return this->borrowingItems.size(); }


	
private:
	std::vector<Borrow> borrowingItems{}; ///< List of borrows.


};

