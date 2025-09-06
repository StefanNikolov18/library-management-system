#pragma once
#include "LibraryItem.h"

/// @class Book
/// @brief Represents a book as a library item with author, year, keywords, and ISBN.
class Book : public virtual LibraryItem
{
public:
	/// @brief Default constructor.
	Book() = default;

	/// @brief Constructor with parameters.
	/// @param genre Genre of the book.
	/// @param title Title.
	/// @param publisher Publisher.
	/// @param rating Rating.
	/// @param description Description.
	/// @param copiesCount Number of copies.
	/// @param author Author.
	/// @param year Year of publication.
	/// @param keywords Keywords.
	/// @param ISBN ISBN number.
	Book(const GENRE& genre, const std::string& title,
		const std::string& publisher, double rating,
		const std::string& description,
		const size_t copiesCount,
		const std::string& author,
		const unsigned year,
		const std::string& keywords,
		const std::string& ISBN);

	/// @brief Prints information about the book.
	/// @brief format: Book|Genre|Title|publisher|rating|description|copies|author|year|keywords|ISBN or N/A
	/// @param os Output stream.
	void print(std::ostream& os) const override;
	/// @brief Reads book information from an input stream.
	/// @param is Input stream.
	void read(std::istream& is) override;

	/// @brief Clones the current object.
	/// @return Pointer to a copy of the object.
	LibraryItem* clone() const override { //clone
		return new Book(*this);
	}

	/// @brief Returns the type of the library item.
	/// @return String "Book".
	std::string getType() const override { return "Book"; }

	/// @brief Prints filtered information about the book.
	/// @param os Output stream.
	void printFiltered(std::ostream& os = std::cout) const override;

	//getters
	const std::string& getAuthor() const override { return this->author; }
	const unsigned getYear() const override { return this->year; }
	const std::string& getKeyWords() const { return this->keywords; }
	const std::string& getISBN() const { return this->ISBN; }

	//setters
	void setAuthor(const std::string& newAuthor) { this->author = newAuthor; }
	void setYear(const unsigned newYear = 2025) { this->year = newYear; }
	void setKeywords(const std::string& newKeywords) { this->keywords = newKeywords; }
	void setISBN(const std::string& newISBN = "") { this->ISBN = newISBN; }

	/// @brief Checks if the object has an ISBN.
	/// @return true if it has ISBN, false otherwise.
	bool hasISBN() const { return this->ISBN != ""; }

	/// @brief Returns ISBN or ISSN (virtual method).
	/// @return ISBN or ISSN as a string.
	const std::string& getIsbnIssn() const override { return ISBN; };

protected:
	/// @brief Checks if a year is valid for Series.
	/// @param year Year.
	/// @return true if the year is valid, false otherwise.
	static bool isValidYear(unsigned year);

	std::string author{};      ///< Author of the book.
	unsigned year = 2025;      ///< Year of publication.
	std::string keywords{};    ///< Keywords.
	std::string ISBN{};        ///< ISBN number.
};
