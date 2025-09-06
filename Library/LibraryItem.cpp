#include "LibraryItem.h"
#include <iostream>

unsigned LibraryItem::NextID = 0;

LibraryItem::LibraryItem(const GENRE& genre, const std::string& title,
	const std::string& publisher, double rating, const std::string& description,
	const size_t copiesCount):
	uniqueID(createUniqueID()),
	genre(isValidGenre(genre) ? genre : GENRE::Unknown),
	title(title),
	publisher(publisher),
	rating(rating),
	description(description),
	copiesCnt(copiesCount)
{
	if (!isValidRating(rating))
		throw std::invalid_argument("Invalid argument!");
}

void LibraryItem::printLibraryItem(std::ostream& os) const
{
	os <<	getType() << '|'
		<<	getGenre() << '|'
		<< getTitle() << '|'
		<< getPublisher() << '|'
		<< getRating() << '|'
		<< getDescription() << '|'
		<< getCopiesCNT() << '|'; //LibraryItem
}

void LibraryItem::readLibraryItem(std::istream& is)
{
	std::string temp{};
	unsigned num = 0;

	if (is.peek() == '|')
		is.ignore();

	is >> num;
	if (isValidGenre(num)) {			//GENRE
		this->setGenre((GENRE)num);
	}
	else {
		this->setGenre(GENRE::Unknown);
	}
		
	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|');			//Title
	this->setTitle(temp);


	std::getline(is, temp, '|');			//Publisher
	this->setPublisher(temp);

	double tempRating = 0.0;

	is >> tempRating;						//Rating
	if (isValidRating(tempRating))
		setRating(tempRating);
	else
		setRating(0.0);

	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|');			//Description
	this->setDescription(temp);

	size_t copiesTemp = 0;
	is >> copiesTemp;
	this->setCopiesCNT(copiesTemp);			//Copies

}


const unsigned LibraryItem::createUniqueID()
{
	return LibraryItem::NextID++;
}

bool LibraryItem::isValidRating(const double rating)
{
	return 0.0 <= rating && rating <= 10.00;
}

std::string genreToString(int genre)
{
	switch (genre) {
	case Fantasy:   return "Fantasy";
	case SciFi:     return "SciFi";
	case Mystery:   return "Mystery";
	case Romance:   return "Romance";
	case Thriller:  return "Thriller";
	case Horror:    return "Horror";
	case Biography: return "Biography";
	default:        return "Unknown";
	}
}
