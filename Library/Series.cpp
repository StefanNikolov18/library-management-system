#include "Series.h"

Series::Series(const GENRE& genre, const std::string& title,
	const std::string& publisher, double rating, const std::string& description,
	const size_t copiesCount, const std::string& author, const unsigned year,
	const std::string& keywords, const std::string& ISBN, const YearAndMonth& date,
	const size_t issueNum, const std::vector<Article>& articles, const std::string& ISSN) :
	LibraryItem(genre, title, publisher, rating, description, copiesCount),
	Book(genre, title, publisher, rating, description, copiesCount, author, year, keywords, ISBN),
	Periodical(genre, title, publisher, rating, description, copiesCount, date, issueNum, articles, ISSN)
{
}

void Series::print(std::ostream& os) const
{
	printLibraryItem(os);
	
	os << getAuthor() << '|'			//Author
		<< getYear() << '|'				//year
		<< getKeyWords() << '|';		//keywords

	if (hasISBN())						//ISBN
		os << this->getISBN() << '|';
	else
		os << "N/A" << '|';

	os << getDate().year << '|'				//year
		<< getDate().month << '|'			//month
		<< getIssueNum() << '|'				//isue num
		<< this->getArticles().size() << '|'; //size

	for (const Article& art : getArticles()) { // Articles
		os << art.title << '|'					//title
			<< art.author << '|'				//author
			<< art.keywords << '|';				//keywords
	}


	if (hasISSN())				//ISSN
		os << getISSN();
	else
		os << "N/A";
	
	
}

void Series::read(std::istream& is)
{
	readLibraryItem(is);

	std::string temp{};
	unsigned year = 0, month = 0;

	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|'); // Author
	setAuthor(temp);

	is >> year;
	if (Book::isValidYear(year))
		setYear(year);
	else
		setYear(); // default

	if (is.peek() == '|')
		is.ignore();

	std::getline(is, temp, '|'); // Keywords
	setKeywords(temp);

	// ISBN
	std::getline(is, temp, '|');
	if (temp == "N/A")
		setISBN();
	else
		setISBN(temp);

	if (is.peek() == '|')
		is.ignore();

	is >> year;
	is.ignore();
	is >> month;
	setYearAndMonth({ year, month });

	if (is.peek() == '|')
		is.ignore();
	size_t issueNum = 0;
	is >> issueNum;
	setIssueNum(issueNum);

	if (is.peek() == '|')
		is.ignore();
	size_t articleCount = 0;
	is >> articleCount;
	is.ignore();

	std::vector<Article> Articles(articleCount);
	for (size_t i = 0; i < articleCount; ++i) {
		std::getline(is, Articles[i].title, '|');
		std::getline(is, Articles[i].author, '|');
		std::getline(is, Articles[i].keywords, '|');
	}
	setArticles(Articles);

	std::getline(is, temp);
	if (temp == "N/A")
		setISSN();
	else
		setISSN(temp);

}

void Series::printFiltered(std::ostream& os) const
{
	os << "[" << this->getUniqueID() << "] "
		<< "Type: " << this->getType() << ", "
		<< "\"" << this->getTitle() << "\""
		<< " by " << this->getAuthor() << ", "
		<< "(" << this->getYear() << "), "
		<< "Issue: " << this->getIssueNum() << ", "
		<< "Genre: " << genreToString(this->getGenre()) << ", "
		<< "Copies: " << this->getCopiesCNT() << ", "
		<< "Rating: " << this->getRating() << "/10\n";
	os << "Description: " << this->getDescription() << '\n';
}
