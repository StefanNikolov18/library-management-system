#pragma once
#include <string>
#include <fstream>
#include <iostream>

/// @enum GENRE
/// @brief Types of genres for library items.
enum GENRE {
	Unknown = -1,
	Fantasy,
	SciFi,
	Mystery,
	Romance,
	Thriller,
	Horror,
	Biography,
	Count
};

/// @brief Checks if an integer is a valid genre.
/// @param x Integer representing a genre.
/// @return true if x is a valid genre, false otherwise.
inline bool isValidGenre(int x) {
	return GENRE::Unknown < x && x < GENRE::Count;
}

/// @brief Converts a genre to a string.
/// @param x Integer corresponding to a genre.
/// @return String with the genre name.
std::string genreToString(int x);

/// @class LibraryItem
/// @brief Abstract base class for library items such as books,periodicals and series.
class LibraryItem
{
public:
	/// @brief Default constructor, creates a unique ID.
	LibraryItem() :uniqueID(createUniqueID()) { };

	/// @brief Constructor with parameters.
	/// @param genre Genre of the item.
	/// @param title Title.
	/// @param publisher Publisher.
	/// @param rating Rating (0.0 - 9.99).
	/// @param description Description.
	/// @param copiesCount Number of copies.
	LibraryItem(const GENRE& genre, const std::string& title, const std::string& publisher,
		double rating, const std::string& description, const size_t copiesCount);

	/// @brief Virtual destructor.
	virtual ~LibraryItem() noexcept = default;

	/// @brief Virtual function to print information.
	/// @param os Output stream, default is std::cout.
	virtual void print(std::ostream& os = std::cout) const = 0;

	/// @brief Virtual function to read data.
	/// @param is Input stream, default is std::cin.
	virtual void read(std::istream& is = std::cin) = 0;

	/// @brief Clones the current object.
	/// @return Pointer to a copy of the object.
	virtual LibraryItem* clone() const = 0;

	/// @brief Get type of the item.
	/// @return String of what type the item is.
	virtual std::string getType() const = 0;

	//getters
	virtual const std::string& getIsbnIssn() const = 0;
	const unsigned getUniqueID() const { return this->uniqueID; }
	const GENRE& getGenre() const { return this->genre; }
	const std::string& getTitle() const { return this->title; }
	const std::string& getPublisher() const { return this->publisher; }
	const double getRating() const { return this->rating; }
	const std::string& getDescription() const { return this->description; }
	const size_t getCopiesCNT() const { return this->copiesCnt; }

	/// @brief Returns the author (defined in derived classes).
	/// @return String with the author's name.
	virtual const std::string& getAuthor() const = 0;
	/// @brief Returns the publication year (defined in derived classes).
	/// @return Integer representing the year.
	virtual const unsigned getYear() const = 0;

	//setters
	void setGenre(const GENRE& newGenre) { this->genre = (isValidGenre(newGenre) ? (GENRE)newGenre : GENRE::Unknown); }
	void setTitle(const std::string& newtitle) { this->title = newtitle; }
	void setPublisher(const std::string& newPublisher) { this->publisher = newPublisher; }
	void setRating(const double newRating) {  (newRating >= 0.0 && newRating <= 9.99) ? this->rating = newRating:this->rating = 0.0; }
	void setDescription(const std::string& newDescription) { this->description = newDescription; }
	void setCopiesCNT(const size_t newCopies) { this->copiesCnt = newCopies; }

	/// @brief Prints the basic data of the library item for derived classes.
	/// @param os Output stream.
	void printLibraryItem(std::ostream& os) const;
	/// @brief Reads the basic data from an input stream for derived classes.
	/// @param is Input stream.
	void readLibraryItem(std::istream& is);

	/// @brief Virtual function for filtered printing, used in the system for derived classes.
	/// @param os Output stream (default std::cout).
	virtual void printFiltered(std::ostream& os = std::cout) const = 0;

	/// @brief Factory function for creating LibraryItem objects.
	/// @param is Input stream from which to read the data.
	/// @return Pointer to the newly created LibraryItem.
	static LibraryItem* LibraryFactory(std::istream& is);

	/// @brief Next unique identifier (static variable).
	static unsigned NextID;

private:
	/// @brief Generates a unique identifier.
	/// @return Integer - unique ID.
	static const unsigned createUniqueID();

	/// @brief Checks if a rating is valid.
	/// @param rating Rating value.
	/// @return true if the rating is valid, false otherwise.
	static bool isValidRating(const double rating);

	/// @brief Unique identifier of the library item.
	const unsigned uniqueID;

	GENRE genre = GENRE::Unknown;       ///< Genre
	std::string title{};                ///< Title
	std::string publisher{};            ///< Publisher
	double rating = 0.0;                ///< Rating
	std::string description{};          ///< Description
	size_t copiesCnt = 1;               ///< Number of copies
};
