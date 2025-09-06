#include "Book.h"
#include "../Date/Date.h"
#include <stdexcept>

Book::Book(const GENRE& genre, const std::string& title, 
	const std::string& publisher, double rating, const std::string& description,
	const size_t copiesCount, const std::string& author, const unsigned year, 
	const std::string& keywords, const std::string& ISBN):
	LibraryItem(genre,title,publisher,rating,description,copiesCount),
	author(author),
	year(year),
	keywords(keywords),
	ISBN(ISBN)
{
	if (!isValidYear(year))
		throw std::invalid_argument("Not valid year!");
}

void Book::print(std::ostream& os) const		// storing in files with |
{
	printLibraryItem(os); //LibraryItems data

	os << getAuthor() << '|'		//Author
		<< getYear() << '|'			//year
		<< getKeyWords() << '|';	//keywords

	if (hasISBN())					//ISBN
		os << ISBN;
	else
		os << "N/A";

}

void Book::read(std::istream& is)
{
	
	readLibraryItem(is);			//LibraryItem

	std::string temp{};
	unsigned year = 0;

	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|');			//Author
	this->setAuthor(temp);

	is >> year;								//year
	if (isValidYear(year))
		this->setYear(year);
	else
		this->setYear();

	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|');			//Keywords
	this->setKeywords(temp);

	std::getline(is, temp);
	if (temp != "N/A") {
		setISBN(temp);
	}
	else {
		setISBN();
	}
		

}

void Book::printFiltered(std::ostream& os) const
{
	os << "[" << this->getUniqueID() << "] "
		<< "Type: " << this->getType() << ", "
		<< "\"" << this->getTitle() << "\" by " << this->author
		<< " (" << this->year << "), "
		<< "Genre: " << genreToString(this->getGenre())
		<< ", Copies: " << this->getCopiesCNT()
		<< ", Rating: " << this->getRating() << "/10\n";
	os << "Description: " << this->getDescription() << '\n';
}

bool Book::isValidYear(unsigned year)
{
	Date today = getTodayDate();
	return year >= 1800 && year <= today.getYear();
}
